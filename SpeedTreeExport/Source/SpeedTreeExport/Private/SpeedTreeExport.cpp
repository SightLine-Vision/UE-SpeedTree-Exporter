// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SpeedTreeExportPrivatePCH.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "SpeedTreeExportStyle.h"
#include "SpeedTreeExportCommands.h"

#include "LevelEditor.h"

static const FName SpeedTreeExportTabName("SpeedTreeExport");

#define LOCTEXT_NAMESPACE "FSpeedTreeExportModule"

void FSpeedTreeExportModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSpeedTreeExportStyle::Initialize();
	FSpeedTreeExportStyle::ReloadTextures();

	FSpeedTreeExportCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSpeedTreeExportCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FSpeedTreeExportModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FSpeedTreeExportModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FSpeedTreeExportModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FSpeedTreeExportModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSpeedTreeExportStyle::Shutdown();

	FSpeedTreeExportCommands::Unregister();
}

FString FSpeedTreeExportModule::GetSectionName(AActor* TargetActor)
{
	FString _Folder = (TargetActor->GetFolderPath()).ToString();

	TArray<FString> Parsed;
	
	_Folder.ParseIntoArray(Parsed, TEXT("/"), false);
	
	return Parsed[Parsed.Num() - 1];
}

bool FSpeedTreeExportModule::isSpeedTree(AActor* TargetActor)
{
	FString _Folder = (TargetActor->GetFolderPath()).ToString();

	TArray<FString> Parsed;

	_Folder.ParseIntoArray(Parsed, TEXT("/"), false);

	return Parsed[0] == "SpeedTree" && Parsed.Num() > 1;
}

void FSpeedTreeExportModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	/*FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FSpeedTreeExportModule::PluginButtonClicked()")),
							FText::FromString(TEXT("SpeedTreeExport.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);*/

	//UE_LOG(LogTemp, Warning, TEXT("EEEEE"));

	FText Title1 = FText::FromString(TEXT("Confirm action"));
	FText DialogText1 = FText::FromString(TEXT("Do you really want to export trees?"));
	if (FMessageDialog::Open(EAppMsgType::YesNo, DialogText1, &Title1) == EAppReturnType::No) return;

	FString FileName = "C:\\temp\\trees.ustf";
	
	ClearIni(FileName);

	AActor* TargetActor = NULL;
	int i = 0;
	for (FSelectionIterator It(*GEditor->GetSelectedActors()); It; ++It)
	{		
		TargetActor = static_cast<AActor*>(*It);

		if (!isSpeedTree(TargetActor)) continue;

		FString TreeName = TargetActor->GetName();
		FString SectionName = GetSectionName(TargetActor);
		
		FVector Location = TargetActor->GetActorLocation();
		FString X = FString::SanitizeFloat(Location.X);
		FString Y = FString::SanitizeFloat(Location.Y);
		FString Z = FString::SanitizeFloat(Location.Z);

		FRotator Rotation = TargetActor->GetActorRotation();
		FString RotationZ = GetFloat(Rotation.Yaw, 2);
		
		FVector Scale = TargetActor->GetActorScale3D();
		FString ScaleZ = GetFloat(Scale.Z, 2);
				
		FString Format = FString::Printf(TEXT("%s %s %s @ %s @ %s"), *X, *Y, *Z, *RotationZ, *ScaleZ);

		GConfig->SetString(*SectionName, *TreeName, *Format, *FileName);
		
		i++;
	}

	GConfig->Flush(false, FileName);

	if (i > 0)
	{
		FText Title2 = FText::FromString(TEXT("Export Success!"));
		FString DialogText2 = FString::Printf(TEXT("Exported %d tree%s"), i, (i < 2) ? "" : "s");
		FMessageDialog::Debugf(FText::FromString(DialogText2), &Title2);
	}
	else
	{
		FText Title3 = FText::FromString(TEXT("Export Failed!"));
		FString DialogText3 = FString::Printf(TEXT("Please select at least %d tree\nfrom \"SpeedTree\\Tree Name\" folder!"), 1);
		FMessageDialog::Debugf(FText::FromString(DialogText3), &Title3);
	}
}

void FSpeedTreeExportModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FSpeedTreeExportCommands::Get().PluginAction);
}

void FSpeedTreeExportModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FSpeedTreeExportCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSpeedTreeExportModule, SpeedTreeExport)
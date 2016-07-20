// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FSpeedTreeExportModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	FString GetSectionName(AActor* TargetActor);
	bool isSpeedTree(AActor* TargetActor);

	static FORCEINLINE FString GetFloat(float TheFloat, int32 Precision, bool IncludeLeadingZero = true)
	{
		FNumberFormattingOptions NumberFormat;					
		NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
		NumberFormat.MaximumIntegralDigits = 10000;
		NumberFormat.MinimumFractionalDigits = Precision;
		NumberFormat.MaximumFractionalDigits = Precision;
		return FText::AsNumber(TheFloat, &NumberFormat).ToString();
	}

	void ClearIni(FString FileName)
	{
		TArray<FString> SectionsNames;
		GConfig->GetSectionNames(*FileName, SectionsNames);

		for (auto& s : SectionsNames)
		{
			GConfig->EmptySection(*s, FileName);
		}

		GConfig->Flush(false, FileName);
	}
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);
	

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
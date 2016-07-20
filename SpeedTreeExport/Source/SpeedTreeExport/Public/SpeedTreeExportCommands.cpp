// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SpeedTreeExportPrivatePCH.h"
#include "SpeedTreeExportCommands.h"

#define LOCTEXT_NAMESPACE "FSpeedTreeExportModule"

void FSpeedTreeExportCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "SpeedTreeExport", "Execute SpeedTreeExport action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE

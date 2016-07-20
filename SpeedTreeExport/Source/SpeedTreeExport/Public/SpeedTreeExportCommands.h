// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "SpeedTreeExportStyle.h"

class FSpeedTreeExportCommands : public TCommands<FSpeedTreeExportCommands>
{
public:

	FSpeedTreeExportCommands()
		: TCommands<FSpeedTreeExportCommands>(TEXT("SpeedTreeExport"), NSLOCTEXT("Contexts", "SpeedTreeExport", "SpeedTreeExport Plugin"), NAME_None, FSpeedTreeExportStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};

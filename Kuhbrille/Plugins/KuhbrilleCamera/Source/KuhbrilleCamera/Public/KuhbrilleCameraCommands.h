// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "KuhbrilleCameraStyle.h"

class FKuhbrilleCameraCommands : public TCommands<FKuhbrilleCameraCommands>
{
public:

	FKuhbrilleCameraCommands()
		: TCommands<FKuhbrilleCameraCommands>(TEXT("KuhbrilleCamera"), NSLOCTEXT("Contexts", "KuhbrilleCamera", "KuhbrilleCamera Plugin"), NAME_None, FKuhbrilleCameraStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};

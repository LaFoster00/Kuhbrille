// Copyright Epic Games, Inc. All Rights Reserved.

#include "KuhbrilleCameraCommands.h"

#define LOCTEXT_NAMESPACE "FKuhbrilleCameraModule"

void FKuhbrilleCameraCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "KuhbrilleCamera", "Bring up KuhbrilleCamera window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE

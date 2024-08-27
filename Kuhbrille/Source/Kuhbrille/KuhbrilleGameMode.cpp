// Copyright Epic Games, Inc. All Rights Reserved.

#include "KuhbrilleGameMode.h"
#include "KuhbrilleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKuhbrilleGameMode::AKuhbrilleGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

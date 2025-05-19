// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoolNinjaGameMode.h"
#include "CoolNinjaCharacter.h"

ACoolNinjaGameMode::ACoolNinjaGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ACoolNinjaCharacter::StaticClass();	
}

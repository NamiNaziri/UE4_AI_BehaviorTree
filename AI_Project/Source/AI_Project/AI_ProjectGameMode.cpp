// Copyright Epic Games, Inc. All Rights Reserved.

#include "AI_ProjectGameMode.h"
#include "AI_ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAI_ProjectGameMode::AAI_ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

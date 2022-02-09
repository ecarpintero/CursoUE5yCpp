// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShooterUEGameModeBase.h"

#include "PlayerCharacter.h"

AShooterUEGameModeBase::AShooterUEGameModeBase()
{
	//Pasamos la ruta del Blueprint para obtener su calse y ponerlo como el Pawn por defecto en el GameMode
	static ConstructorHelpers::FClassFinder<APlayerCharacter> PlayerCharacterBP (TEXT("/Game/Blueprints/PlayerCharacterBP"));
	if(PlayerCharacterBP.Class != NULL)
	{
		DefaultPawnClass = PlayerCharacterBP.Class;
	}
}

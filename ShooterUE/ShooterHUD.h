// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerCharacter.h"
#include "ShooterHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERUE_API AShooterHUD : public AHUD
{
	GENERATED_BODY()

	UTexture2D* CrossTextureBottom;
	UTexture2D* CrossTextureTop;
	UTexture2D* CrossTextureLeft;
	UTexture2D* CrossTextureRight;


	APlayerCharacter* Player;
	enum CrossHairMoveDirection {X,Y};

private:
	void MoveCrossHairs(float ScreenXSize,float ScreenYSize,UTexture2D* CrossTexture,
		float Location,int Direction, CrossHairMoveDirection MoveDirection);

protected:
	virtual void BeginPlay() override;

public:
	AShooterHUD();

	virtual void DrawHUD() override;
	
};

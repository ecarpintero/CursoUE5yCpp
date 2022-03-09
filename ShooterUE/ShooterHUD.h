// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERUE_API AShooterHUD : public AHUD
{
	GENERATED_BODY()

	UTexture2D* CrossTexture;
public:
	AShooterHUD();

	virtual void DrawHUD() override;
	
};

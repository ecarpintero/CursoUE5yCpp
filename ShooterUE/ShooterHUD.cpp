// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterHUD.h"

#include "Engine/Canvas.h"

AShooterHUD::AShooterHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> Crosshair (TEXT("/Game/HUD/Crosshairs/Cross_Q_9"));
	if(Crosshair.Object)
	{
		CrossTexture = Crosshair.Object;
	}
}

void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();
	if(CrossTexture)
	{
		float ScreenXSize = (Canvas->SizeX/2)-32;
		float ScreenYSize = ((Canvas->SizeY/2)-50)-32;
		DrawTexture(CrossTexture,ScreenXSize,ScreenYSize,64,64,0,0,1,1);
	}
}

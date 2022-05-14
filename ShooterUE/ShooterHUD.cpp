// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"



AShooterHUD::AShooterHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairBottom (TEXT("/Game/HUD/Crosshairs/Crosshair_Quad_9_Bottom"));
	if(CrosshairBottom.Object)
	{
		CrossTextureBottom = CrosshairBottom.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTop(TEXT("/Game/HUD/Crosshairs/Crosshair_Quad_9_Top"));
	if (CrosshairTop.Object)
	{
		CrossTextureTop = CrosshairTop.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairLeft(TEXT("/Game/HUD/Crosshairs/Crosshair_Quad_9_Left"));
	if (CrosshairLeft.Object)
	{
		CrossTextureLeft= CrosshairLeft.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairRight(TEXT("/Game/HUD/Crosshairs/Crosshair_Quad_9_Right"));
	if (CrosshairRight.Object)
	{
		CrossTextureRight = CrosshairRight.Object;
	}
}

void AShooterHUD::MoveCrossHairs(float ScreenXSize, float ScreenYSize, UTexture2D* CrossTexture, float Location, int Direction, CrossHairMoveDirection MoveDirection)
{
	float CrossHairSpreadMax = 64.0f;
	float CrossHairSpreadMultiplier = 0;
	if (Player) {
		CrossHairSpreadMultiplier = Player->GetCrosshairSpreadMultiplier();
	}
	FVector2D CrossHairBaseCenter = FVector2D(ScreenXSize, ScreenYSize);
	switch (MoveDirection)
	{
		case X:
			CrossHairBaseCenter.X += ((CrossHairSpreadMax * CrossHairSpreadMultiplier) * Direction) + Location;
			break;
		case Y:
			CrossHairBaseCenter.Y += ((CrossHairSpreadMax * CrossHairSpreadMultiplier) * Direction) + Location;
			break;
	}
	DrawTexture(CrossTexture, CrossHairBaseCenter.X, CrossHairBaseCenter.Y, 64, 64, 0, 0, 1, 1);
}

void AShooterHUD::BeginPlay()
{
	if (Player == nullptr){
		Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	}
}

void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();
	if(CrossTextureBottom)
	{
		float ScreenXSize = (Canvas->SizeX/2)-32;
		float ScreenYSize = ((Canvas->SizeY/2)-50)-32;

		MoveCrossHairs(ScreenXSize, ScreenYSize, CrossTextureBottom, 4.0f, 1.0f, CrossHairMoveDirection::Y);
		MoveCrossHairs(ScreenXSize, ScreenYSize, CrossTextureTop, -4.0f, -1.0f, CrossHairMoveDirection::Y);
		MoveCrossHairs(ScreenXSize, ScreenYSize, CrossTextureLeft, -4.0f, -1.0f, CrossHairMoveDirection::X);
		MoveCrossHairs(ScreenXSize, ScreenYSize, CrossTextureRight, 4.0f, 1.0f, CrossHairMoveDirection::X);
	}
}

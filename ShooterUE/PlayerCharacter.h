// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SHOOTERUE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	//Con este componente hacemos que la camara rote al rededor del personaje
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera, meta=(AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	//Camara del personaje
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera, meta=(AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	//Angulos de rotacion de la camara
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera, meta=(AllowPrivateAccess = "true"))
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera, meta=(AllowPrivateAccess = "true"))
	float BaseLookUpRate;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Camera, meta=(AllowPrivateAccess = "true"))
	float HipTurnRate;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Camera, meta=(AllowPrivateAccess = "true"))
	float HipLookUpRate;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Camera, meta=(AllowPrivateAccess = "true"))
	float AimingTurnRate;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Camera, meta=(AllowPrivateAccess = "true"))
	float AimingLookUpRate;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Camera, meta=(AllowPrivateAccess = "true"),meta=(ClampMin="0.0",ClampMax="1.0",UIMin="0.0",UIMax="1.0"))
	float MouseHipTurnRate;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Camera, meta=(AllowPrivateAccess = "true"),meta=(ClampMin="0.0",ClampMax="1.0",UIMin="0.0",UIMax="1.0"))
	float MouseHipLookUpRate;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Camera, meta=(AllowPrivateAccess = "true"),meta=(ClampMin="0.0",ClampMax="1.0",UIMin="0.0",UIMax="1.0"))
	float MouseAimingTurnRate;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Camera, meta=(AllowPrivateAccess = "true"),meta=(ClampMin="0.0",ClampMax="1.0",UIMin="0.0",UIMax="1.0"))
	float MouseAimingLookUpRate;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Sound, meta=(AllowPrivateAccess = "true"))
	USoundCue* FireSound;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Animation, meta=(AllowPrivateAccess = "true"))
	UAnimMontage* FireAnimMontage;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=FX, meta=(AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=FX, meta=(AllowPrivateAccess = "true"))
	UParticleSystem* MuzzlesParticles;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=FX, meta=(AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera, meta=(AllowPrivateAccess = "true"))
	bool bAiming;

	float CameraDeafultFOV;
	float CameraZoomedFOV;
	float CameraCurrentFOV;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivateAccess="true"))
	float SpeedZoomed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshair, meta = (AllowPrivateAccess = "true"))
	float CrosshairsSpreadMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshair, meta = (AllowPrivateAccess = "true"))
	float CrosshairsVelocityFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshair, meta = (AllowPrivateAccess = "true"))
	float CrosshairsAirFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshair, meta = (AllowPrivateAccess = "true"))
	float CrosshairsInAimFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshair, meta = (AllowPrivateAccess = "true"))
	float CrosshairsShootingFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bFiringBullet;
	FTimerHandle CrossHairTimeHandle;

public:
	// Constructor
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Movmiento vertical del jugador
	void MoveForward(float Value);
	//Movmiento lateral del jugador
	void MoveRight(float Value);
	//Rotacion lateral de camara
	void TurnAtRate(float Rate);
	//Rotacion horizontal de la camara
	void LookAtRate(float Rate);

	void Turn(float Value);
	void LookUp(float Value);
	
	void Fire();

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult);
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	void AiminButtonPressed();
	void AiminButtonReleased();

	void CameraInterpZoom(float DeltaTime);
	void SetLookRate();

private:
	void FireLineCast(FName SocketName);
	void CalculateCrosshairsSpread(float Deltatime);

	void StartCrosshairBulletFire();
	void FinishCrosshairBulletFire();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetAiming() const{ return bAiming;}
	float GetCrosshairSpreadMultiplier() const;
};

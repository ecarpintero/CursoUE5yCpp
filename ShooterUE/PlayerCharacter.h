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
	void Fire();

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult);
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

private:
	void FireLineCast(FName SocketName);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};

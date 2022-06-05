// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter() :
	HipTurnRate(45.f),HipLookUpRate(90.f),AimingTurnRate(20.f),AimingLookUpRate(20.f),
	MouseHipTurnRate(1.0f),MouseHipLookUpRate(1.0f),MouseAimingTurnRate(0.2f),MouseAimingLookUpRate(0.2f),
	//Valores por defecto de las variables
	BaseTurnRate(45.f),BaseLookUpRate(45.f),CameraZoomedFOV(35.f),SpeedZoomed(20)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Creamos los componetes y asignamos valores por defecto
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 180;
	//Rota el personaje cuando rota el SpringArmComponent
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0,50,70);
	//Camara del jeugo
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//Lo unimos al SpringArmComponent
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//El personaje rota con la camara en el eje horizontal
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(FollowCamera)
	{
		CameraDeafultFOV= FollowCamera->FieldOfView;
		CameraCurrentFOV = CameraDeafultFOV;
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	//Movimiento vertical del jugador segun la rotacion del jugador
	if(Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction,Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	//Movimiento horizontal del jugador segun la rotacion del jugador
	if(Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction,Value);
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	//Añadiendo la rotacion vertical a la camara
	AddControllerYawInput(Rate*BaseTurnRate*GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookAtRate(float Rate)
{
	//Añadiendo la rotacion horizontal a la camara
	AddControllerPitchInput(Rate*BaseLookUpRate*GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Turn(float Value)
{
	float TurnScaleFactor;

	if(bAiming)
	{
		TurnScaleFactor = MouseAimingTurnRate;
	}else
	{
		TurnScaleFactor = MouseHipTurnRate;
	}
	
	AddControllerYawInput(Value*TurnScaleFactor);
}

void APlayerCharacter::LookUp(float Value)
{
	float TurnScaleFactor;

	if(bAiming)
	{
		TurnScaleFactor = MouseAimingLookUpRate;
	}else
	{
		TurnScaleFactor = MouseHipLookUpRate;
	}
	
	AddControllerPitchInput(Value*TurnScaleFactor);
}

void APlayerCharacter::Fire()
{
	if(FireSound)
	{
		UGameplayStatics::PlaySound2D(this,FireSound);
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && FireAnimMontage)
	{
		AnimInstance->Montage_Play(FireAnimMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}

	FireLineCast("Fire_SockeL");
	FireLineCast("Fire_SocketR");

	StartCrosshairBulletFire();

	if (ShooterCameraShake) {
		UMatineeCameraShake::StartMatineeCameraShake(GetWorld()->GetFirstPlayerController()->PlayerCameraManager,
			ShooterCameraShake,
			1.0f,
			ECameraShakePlaySpace::CameraLocal,
			FRotator::ZeroRotator);
	}
}

bool APlayerCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult)
{
	FVector OutBeamLocation;
	FHitResult CrosshaitHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs(CrosshaitHitResult,OutBeamLocation);
	if(bCrosshairHit)
	{
		OutBeamLocation = CrosshaitHitResult.Location;
	}

	const FVector WeaponTraceStart = MuzzleSocketLocation;
	const FVector WeaponTraceEnd = OutBeamLocation;

	GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		WeaponTraceStart,
		WeaponTraceEnd,
		ECollisionChannel::ECC_Visibility);
	if(!OutHitResult.bBlockingHit)
	{
		OutHitResult.Location = OutBeamLocation;
		return false;
	}

	return true;
}

bool APlayerCharacter::TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation2D = FVector2D(ViewportSize.X/2,ViewportSize.Y/2);
	FVector CrosshairWorldLocation;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
	UGameplayStatics::GetPlayerController(this,0),
	CrosshairLocation2D,
	CrosshairWorldLocation,
	CrosshairWorldDirection
	);
	if(bScreenToWorld)
	{
		const FVector Start = CrosshairWorldLocation;
		const FVector End = Start + (CrosshairWorldDirection*50000.f);
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(OutHitResult,Start,End,ECollisionChannel::ECC_Visibility);
		if(OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}

	return false;
}

void APlayerCharacter::AiminButtonPressed()
{
	bAiming = true;
	SetLookRate();
}

void APlayerCharacter::AiminButtonReleased()
{
	bAiming = false;
	SetLookRate();
}

void APlayerCharacter::CameraInterpZoom(float DeltaTime)
{
	if(FollowCamera)
	{
		if(bAiming)
		{
			CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV,CameraZoomedFOV,DeltaTime,SpeedZoomed);
		}else
		{
			CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV,CameraDeafultFOV,DeltaTime,SpeedZoomed);
		}
		FollowCamera->SetFieldOfView(CameraCurrentFOV);
	}
}

void APlayerCharacter::SetLookRate()
{
	if(bAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void APlayerCharacter::FireLineCast(FName SocketName)
{
	const USkeletalMeshSocket* Fire_Socket = GetMesh()->GetSocketByName(SocketName);
	if(Fire_Socket)
	{
		const FTransform Fire_SocketTransform = Fire_Socket->GetSocketTransform(GetMesh());

		if(MuzzlesParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),MuzzlesParticles,
				Fire_SocketTransform.GetLocation());
		}
		FHitResult OutHitResult;
		bool bBeamEnd = GetBeamEndLocation(Fire_SocketTransform.GetLocation(),OutHitResult);
		if(bBeamEnd)
		{
			if(ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactParticles,OutHitResult.ImpactPoint);
			}

			if(BeamParticles)
			{
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
				BeamParticles,Fire_SocketTransform);
				if(Beam)
				{
					Beam->SetVectorParameter(FName("Target"),OutHitResult.ImpactPoint);
				}
			}
		}
		
		/*FHitResult FireHit;
		const FVector Start = Fire_SocketTransform.GetLocation();
		const FQuat Rotation = Fire_SocketTransform.GetRotation();
		const FVector RotationAxis = Rotation.GetAxisX();
		const FVector End = (Start+RotationAxis*50000.f);
		GetWorld()->LineTraceSingleByChannel(FireHit,Start,End,ECC_Visibility);
		if(FireHit.bBlockingHit)
		{
			DrawDebugLine(GetWorld(),Start,End,FColor::Red,false,2.f);
			DrawDebugPoint(GetWorld(),FireHit.ImpactPoint,30,FColor::Red,false,2.f);

			if(ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactParticles,FireHit.ImpactPoint);
			}
		}*/
	}
}

void APlayerCharacter::CalculateCrosshairsSpread(float Deltatime)
{
	FVector2D WalkSpreadRange(0.0f,600.0f);
	FVector2D VelocityMultiplierRange = FVector2D(0.0f, 1.0f);
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.0f;

	CrosshairsVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpreadRange, VelocityMultiplierRange,Velocity.Size());

	if (GetCharacterMovement()->IsFalling()) {
		CrosshairsAirFactor = FMath::FInterpTo(CrosshairsAirFactor,2.25f,Deltatime,2.25f);
	}
	else {
		CrosshairsAirFactor = FMath::FInterpTo(CrosshairsAirFactor,0.0f,Deltatime,30.f);
	}

	if (bAiming) {
		CrosshairsInAimFactor = FMath::FInterpTo(CrosshairsInAimFactor, 0.6f, Deltatime, 30.0f);
	}
	else {
		CrosshairsInAimFactor = FMath::FInterpTo(CrosshairsInAimFactor, 0.0f, Deltatime, 30.0f);
	}

	if (bFiringBullet) {
		CrosshairsShootingFactor = FMath::FInterpTo(CrosshairsShootingFactor, 0.3f, Deltatime, 60.f);
	}
	else {
		CrosshairsShootingFactor = FMath::FInterpTo(CrosshairsShootingFactor, 0.0f, Deltatime, 60.f);
	}

	CrosshairsSpreadMultiplier = 0.5f + CrosshairsVelocityFactor+CrosshairsAirFactor-CrosshairsInAimFactor+CrosshairsShootingFactor;
}

void APlayerCharacter::StartCrosshairBulletFire()
{
	bFiringBullet = true;
	GetWorldTimerManager().SetTimer(CrossHairTimeHandle, this, &APlayerCharacter::FinishCrosshairBulletFire, 0.05f);
}

void APlayerCharacter::FinishCrosshairBulletFire()
{
	bFiringBullet = false;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CameraInterpZoom(DeltaTime);

	CalculateCrosshairsSpread(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Movimiento Input del jugador
	PlayerInputComponent->BindAxis("MoveForward",this,&APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&APlayerCharacter::MoveRight);

	//Movimiento camara teclas direccion
	PlayerInputComponent->BindAxis("TurnRate",this,&APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate",this,&APlayerCharacter::LookAtRate);
	//Movimiento camara Raton
	PlayerInputComponent->BindAxis("Turn",this,&APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp",this,&APlayerCharacter::LookUp);

	//Salto del jugador
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&APlayerCharacter::Fire);

	PlayerInputComponent->BindAction("AiminButton",IE_Pressed,this,&APlayerCharacter::AiminButtonPressed);
	PlayerInputComponent->BindAction("AiminButton",IE_Released,this,&APlayerCharacter::AiminButtonReleased);

}

float APlayerCharacter::GetCrosshairSpreadMultiplier() const
{
	return CrosshairsSpreadMultiplier;
}


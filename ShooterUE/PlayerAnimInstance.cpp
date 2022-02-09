// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	if (PlayerCharacter == nullptr) {
		PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}
	else {
		FVector Velocity = PlayerCharacter->GetVelocity();
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();

		if (PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f) {
			bIsAccelerating = true;
		}
		else {
			bIsAccelerating = false;
		}

		//bIsAccelerating = (PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f);
	}
}

void UPlayerAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}
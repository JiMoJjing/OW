#include "OWAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

UOWAnimInstance::UOWAnimInstance()
{
	MovingThreshold = 10.f;
	JumpingThreshold = 100.f;
	GroundSpeed = 0.f;

	Velocity = FVector::ZeroVector;
	bIsIdle = true;
	bIsFalling = false;
	bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
	bIsMontagePlaying = false;
}

void UOWAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ACharacter>(GetOwningActor());

	if(OwnerCharacter)
	{
		MovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UOWAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(MovementComponent)
	{
		//const FRotator ControlRotation = OwnerCharacter->GetControlRotation();
		const FRotator ActorRotation = OwnerCharacter->GetActorRotation();
		
		Velocity = MovementComponent->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsFalling = MovementComponent->IsFalling();
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
		JumpCurrentCount = OwnerCharacter->JumpCurrentCount;
		bIsMontagePlaying = IsAnyMontagePlaying();
		if(bIsIdle)
		{
			MoveDirection = 0.f;
		}
		else
		{
			MoveDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, ActorRotation);
		}
	}
}

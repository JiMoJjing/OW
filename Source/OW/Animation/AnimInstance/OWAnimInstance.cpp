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
		const FVector LastInputVec = MovementComponent->GetLastInputVector();
		
		Velocity = MovementComponent->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsFalling = MovementComponent->IsFalling();
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
		JumpCurrentCount = OwnerCharacter->JumpCurrentCount;
		bIsMontagePlaying = IsAnyMontagePlaying();

		// FString str = FString::Printf(TEXT("%f"), GroundSpeed);
		// GEngine->AddOnScreenDebugMessage(6, 0.02f, FColor::Yellow, str);
		// if(bIsIdle)
		// {
		// 	MoveDirection = 0.f;
		// }
		// else if (GroundSpeed < 100.f)
		// {
		// 	MoveDirection = UKismetAnimationLibrary::CalculateDirection(LastInputVec, ActorRotation);
		// }
		// else
		// {
		// 	MoveDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, ActorRotation);
		// 	FString str2 = FString::Printf(TEXT("%f"), MoveDirection);
		// 	GEngine->AddOnScreenDebugMessage(5, 0.02f, FColor::Yellow, str2);
		// }
		MoveDirection = UKismetAnimationLibrary::CalculateDirection(LastInputVec, ActorRotation);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GenjiSwiftStrikeComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "OW/Character/OWCharacterPlayable.h"


UGenjiSwiftStrikeComponent::UGenjiSwiftStrikeComponent() : SwiftStrikeDistance(1884.f), SwiftStrikeSpeed(5000.f), SwiftStrikeStartLocation(FVector::ZeroVector),
SwiftStrikeEndLocation(FVector::ZeroVector), SwiftStrikeHitNormalProjection(FVector::ZeroVector), HitNormalProjectionInterpSpeed(10.f), CheckDoubleJump(false),
CapsuleSize2D(0.f, 0.f)
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SwiftStrikeMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_Swiftstrike.AM_Genji_Swiftstrike'"));
	if(SwiftStrikeMontageRef.Object)
	{
		AbilityMontage = SwiftStrikeMontageRef.Object;
	}

	CooldownTime = 8.f;
	AbilityType = EAbilityType::EAT_AbilityOne;
}

void UGenjiSwiftStrikeComponent::BeginPlay()
{
	Super::BeginPlay();

	CapsuleSize2D = FVector2D(PlayableCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius(), PlayableCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	PlayableCharacter->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UGenjiSwiftStrikeComponent::OnCapsuleComponentHit);
	PlayableCharacter->GetCharacterMovement()->MaxFlySpeed = SwiftStrikeSpeed;
}

void UGenjiSwiftStrikeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGenjiSwiftStrikeComponent::AbilityStart()
{
	Super::AbilityStart();

	SwiftStrikeStartSetting();

	PlayAbilityMontage();
}

void UGenjiSwiftStrikeComponent::AbilityEnd()
{
	Super::AbilityEnd();

	SwiftStrikeEndSetting();

	CooldownStart();
}

void UGenjiSwiftStrikeComponent::SwiftStrikeStartSetting()
{
	// Set Start Location, End Location -
	// Check DoubleJump -
	// Set Ignore Input true -
	// Set MovementMode Flying -
	// Set MaxAcceleration 1000000.f -
	// NotifyState DELEGATE Bind -
	
	// Capsule Collision Response Change
	// Collider Collision On
	
	SetSwiftStrikeStartLocation();
	SetSwiftStrikeEndLocation();

	CheckDoubleJump = PlayableCharacter->JumpCurrentCount == PlayableCharacter->JumpMaxCount;

	PlayableCharacter->SetIgnoreInput(true);
	PlayableCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	PlayableCharacter->GetCharacterMovement()->MaxAcceleration = 1000000.f;
	PlayableCharacter->OnAnimNotifyState.AddUObject(this, &UGenjiSwiftStrikeComponent::SwiftStrikeUptade);
	PlayableCharacter->OnAnimNotifyEnd.AddUObject(this, &UGenjiSwiftStrikeComponent::AbilityEnd);
	
	if(AbilityMontage)
	{
		AbilityMontage->bEnableAutoBlendOut = false;
	}
}

void UGenjiSwiftStrikeComponent::SwiftStrikeEndSetting()
{
	// Set MovementMode Falling -
	// Set MaxAcceleration 2048.f -
	// StopMovementImmediately -
	// Set Ignore Input false -
	// Capsule Size RE -
	// FVector Reset -
	// NotifyState DELEGATE Unbind
	// Collider Collision off 
	// Capsule Collision Response Change

	PlayableCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	PlayableCharacter->GetCharacterMovement()->MaxAcceleration = 2048.f;
	PlayableCharacter->GetCharacterMovement()->StopMovementImmediately();
	PlayableCharacter->SetIgnoreInput(false);
	PlayableCharacter->OnAnimNotifyState.RemoveAll(this);
	PlayableCharacter->OnAnimNotifyEnd.RemoveAll(this);

	if(CheckDoubleJump)
	{
		PlayableCharacter->JumpCurrentCount = 2;
	}

	CheckDoubleJump = false;

	SwiftStrikeStartLocation = FVector::ZeroVector;
	SwiftStrikeEndLocation = FVector::ZeroVector;
	SwiftStrikeHitNormalProjection = FVector::ZeroVector;

	AbilityMontage->bEnableAutoBlendOut = true;
	PlayAbilityMontage_JumpToSection(TEXT("Section_02"));
}

void UGenjiSwiftStrikeComponent::SetSwiftStrikeStartLocation()
{
	SwiftStrikeStartLocation = PlayableCharacter->GetActorLocation();
}

void UGenjiSwiftStrikeComponent::SetSwiftStrikeEndLocation()
{
	FHitResult HitResult;
	FVector TraceEndLocation;

	bool bLineTrace = PlayableCharacter->TraceUnderCrosshair(SwiftStrikeDistance, HitResult, TraceEndLocation, ECC_Visibility);
	if(bLineTrace)
	{
		const FVector HitNormal = HitResult.Normal;
		const FVector AdjustedVector(HitNormal.X * CapsuleSize2D.X, HitNormal.Y * CapsuleSize2D.X, HitNormal.Z * CapsuleSize2D.Y);
		SwiftStrikeEndLocation = HitResult.Location + AdjustedVector;
	}
	else
	{
		SwiftStrikeEndLocation = TraceEndLocation;
	}

#if WITH_EDITOR
	// Draw Debug
	DrawDebugSphere(GetWorld(), SwiftStrikeEndLocation, 10.f, 20, FColor::Green, false, 5.f, 0U, 1.f);
	DrawDebugLine(GetWorld(), PlayableCharacter->GetActorLocation(), SwiftStrikeEndLocation, FColor::Green, false, 5.f, 0U, 1.f);
#endif
}

void UGenjiSwiftStrikeComponent::SwiftStrikeUptade(float DeltaTime)
{
	FVector ActorLocation = PlayableCharacter->GetActorLocation();
	FVector Direction = SwiftStrikeEndLocation - ActorLocation;
	Direction.Normalize();

	Direction += SwiftStrikeHitNormalProjection;
	Direction.Normalize();
	
	PlayableCharacter->AddMovementInput(Direction, 1.f, true);

	SwiftStrikeHitNormalProjection = FMath::VInterpTo(SwiftStrikeHitNormalProjection, FVector::ZeroVector, DeltaTime, HitNormalProjectionInterpSpeed);

	FVector EndLocation = SwiftStrikeEndLocation;
	EndLocation.Z = 0.f;
	ActorLocation.Z = 0.f;
	if(ActorLocation.Equals(EndLocation, 20.f))
	{
		AbilityEnd();
	}
}

void UGenjiSwiftStrikeComponent::OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if(Hit.bBlockingHit && AbilityState == EAbilityState::EAS_Active)
	{
		const FVector HitNormal = Hit.Normal;
		const FVector ActorForwardVector = PlayableCharacter->GetActorForwardVector();
		// 평면 투영 벡터 공식을 사용한 방법 Vp = V - (V dot N) * N
		SwiftStrikeHitNormalProjection = ActorForwardVector - FVector::DotProduct(ActorForwardVector, HitNormal) * HitNormal;
	}
}
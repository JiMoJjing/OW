// Fill out your copyright notice in the Description page of Project Settings.


#include "OWCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "OW/Interface/OWApplyDamageInterface.h"
#include "OW/Status/HPComponent.h"

AOWCharacterBase::AOWCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->GravityScale = 0.75f;
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->MaxAcceleration = 2048.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->CanCharacterStepUpOn = ECB_Yes;

	// HPComponent
	HPComponent = CreateDefaultSubobject<UHPComponent>(TEXT("HPComponent"));
}

void AOWCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	MeshRelativeLocation = GetMesh()->GetRelativeLocation();
	MeshRelativeRotation = GetMesh()->GetRelativeRotation();
}

float AOWCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	bool bIsHeadShot = false;
	if(DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		FName HitBoneName = PointDamageEvent->HitInfo.BoneName;
		if(HitBoneName == FName(TEXT("head")) || HitBoneName == FName(TEXT("neck_01")))
		{
			ActualDamage *= 2;
			bIsHeadShot = true;
		}
	}
	
	if(nullptr != HPComponent)
	{
		ActualDamage = HPComponent->TakeDamage(ActualDamage);
		if(HPComponent->IsDead())
		{
			CharacterDeath();

			// TO DO : Send GameModeBase CharacterDeath for KillLog
		}
	}

	if(nullptr != EventInstigator)
	{
		if(IOWApplyDamageInterface* ApplyDamageInterface = Cast<IOWApplyDamageInterface>(EventInstigator))
		{
			ApplyDamageInterface->ApplyDamageSuccess(ActualDamage, bIsHeadShot);
		}
	}
	
	return ActualDamage;
}

void AOWCharacterBase::CharacterDeath()
{
	GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::NoCollision));
	GetMesh()->SetCollisionProfileName(FName(TEXT("Ragdoll")));
	GetMesh()->SetSimulatePhysics(true);
	
}

void AOWCharacterBase::CharacterRevive()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	GetMesh()->SetRelativeLocation(MeshRelativeLocation);
	GetMesh()->SetRelativeRotation(MeshRelativeRotation);
}

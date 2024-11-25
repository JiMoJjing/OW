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
	GetCapsuleComponent()->CanCharacterStepUpOn = ECB_Yes;

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
	GetMesh()->CanCharacterStepUpOn = ECB_No;
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// HPComponent
	HPComponent = CreateDefaultSubobject<UHPComponent>(TEXT("HPComponent"));

	BodyCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body"));
	HeadCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Head"));
	HeadCollision->ComponentTags.Add(TEXT("Head"));
	
	CollisionArray.AddUnique(BodyCollision);
	CollisionArray.AddUnique(HeadCollision);

}

void AOWCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	MeshRelativeLocation = GetMesh()->GetRelativeLocation();
	MeshRelativeRotation = GetMesh()->GetRelativeRotation();
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float AOWCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	bool bIsHeadShot = false;
	
	if(DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		UPrimitiveComponent* HitComponent = PointDamageEvent->HitInfo.GetComponent();
		if(HitComponent->ComponentHasTag(TEXT("Head")))
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

			// AddImpulse
			if(DamageCauser)
			{
				const FVector DamagedLocation = DamageCauser->GetActorLocation();
				FVector ImpulseDirection = (GetActorLocation() - DamagedLocation).GetSafeNormal();
				GetMesh()->AddImpulse(ImpulseDirection * 100000.f);
			}

			// Send KillSuccess to Instigator
			if(IOWApplyDamageInterface* ApplyDamageInterface = Cast<IOWApplyDamageInterface>(EventInstigator))
			{
				ApplyDamageInterface->KillSuccess();
			}
			
			// TODO : Send GameModeBase CharacterDeath for KillLog
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
	
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AOWCharacterBase::CharacterRevive()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetRelativeRotation(MeshRelativeRotation);
	GetMesh()->SetRelativeLocation(MeshRelativeLocation);
	
	SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	HPComponent->HPFullRecovery();
}

void AOWCharacterBase::SetCollisionEnabled(ECollisionEnabled::Type InType)
{ 
	for(auto& Collision : CollisionArray)
	{
		Collision->SetCollisionEnabled(InType);
	}
}

void AOWCharacterBase::SetCollisionProfileName(FName InCollisionProfileName)
{
	for(auto& Collision : CollisionArray)
	{
		Collision->SetCollisionProfileName(InCollisionProfileName);
	}
}
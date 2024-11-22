// Fill out your copyright notice in the Description page of Project Settings.


#include "GenjiShurikenComponent.h"

#include "OW/Ability/AbilityManagerComponent.h"
#include "OW/Character/OWCharacterPlayable.h"
#include "OW/Collision/OWCollisionProfile.h"
#include "OW/Projectile/OWProjectileBase.h"


UGenjiShurikenComponent::UGenjiShurikenComponent() : PrimaryFireCooldown(0.88f), SecondaryFireCooldown(0.68f),PoolSize(30), ExtraPoolSize(30),PoolIndex(0)
{

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PrimaryFireMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_PrimaryFire.AM_Genji_PrimaryFire'"));
	if(PrimaryFireMontageRef.Object)
	{
		AbilityMontage = PrimaryFireMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SecondaryFireMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_SecondaryFire.AM_Genji_SecondaryFire'"));
	if(PrimaryFireMontageRef.Object)
	{
		SecondAbilityMontage = SecondaryFireMontageRef.Object;
	}

	static ConstructorHelpers::FClassFinder<AOWProjectileBase> ProjectileClassRef(TEXT("/Game/OW/Genji/Projectile/BP_Shuriken.BP_Shuriken_C"));
	if(ProjectileClassRef.Class)
	{
		ProjectileClass = ProjectileClassRef.Class;
	}

	AbilityType = EAbilityType::EAT_PrimaryFire;
}

void UGenjiShurikenComponent::BeginPlay()
{
	Super::BeginPlay();

	if(ProjectileClass)
	{
		UWorld* World = GetOwner()->GetWorld();
		FActorSpawnParameters ActorSpawnParameter;
		ActorSpawnParameter.Instigator = PlayableCharacter;
		ActorSpawnParameter.Owner = PlayableCharacter;
		
		for(uint8 index = 0; index < PoolSize + ExtraPoolSize; index++)
		{
			AOWProjectileBase* Projectile = World->SpawnActor<AOWProjectileBase>(ProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator, ActorSpawnParameter);

			if(Projectile)
			{
				ProjectilePool.Add(Projectile);
			}
		}
	}
}

void UGenjiShurikenComponent::AbilityStart()
{
	Super::AbilityStart();

	PlayableCharacter->OnAnimNotify.AddUObject(this, &UGenjiShurikenComponent::PrimaryFire);

	SetCooldown(PrimaryFireCooldown);
	CooldownStart();
	
	PlayAbilityMontage(AbilityMontage);
}

void UGenjiShurikenComponent::AbilityEnd()
{
	Super::AbilityEnd();

	PlayableCharacter->OnAnimNotify.RemoveAll(this);
	PlayableCharacter->OnAnimNotifyEnd.RemoveAll(this);
}

void UGenjiShurikenComponent::CooldownEnd()
{
	Super::CooldownEnd();

	AbilityEnd();
}

void UGenjiShurikenComponent::UseSecondAbility()
{
	if(CanUseAbility())
	{
		SecondAbilityStart();
	}
}

void UGenjiShurikenComponent::SecondAbilityStart()
{
	AbilityManagerComponent->AbilityStart(AbilityType);
	SetAbilityState(EAbilityState::EAS_Active);

	PlayableCharacter->OnAnimNotify.AddUObject(this, &UGenjiShurikenComponent::SecondaryFire);
	
	SetCooldown(SecondaryFireCooldown);
	CooldownStart();
	PlayAbilityMontage(SecondAbilityMontage);
}


void UGenjiShurikenComponent::PrimaryFire()
{
	FVector Direction;
	PlayableCharacter->GetDirectionToCrosshair(PlayableCharacter->GetActorLocation(), Direction, OWTEAM1TRACE);
	
	AOWProjectileBase* ProjectileBase = GetProjectileFromPool();

	if(ProjectileBase)
	{
		ProjectileBase->ActivateProjectile(GetOwner()->GetActorLocation(), Direction);
	}
}

void UGenjiShurikenComponent::SecondaryFire()
{
	FVector Direction;
    PlayableCharacter->GetDirectionToCrosshair(PlayableCharacter->GetActorLocation(), Direction, OWTEAM1TRACE);
	const FVector LeftDirection = Direction.RotateAngleAxis(-5.f, FVector::UpVector);
	const FVector RightDirection = Direction.RotateAngleAxis(5.f, FVector::UpVector);

	AOWProjectileBase* ProjectileBase1 = GetProjectileFromPool();
	AOWProjectileBase* ProjectileBase2 = GetProjectileFromPool();
	AOWProjectileBase* ProjectileBase3 = GetProjectileFromPool();

	if(ProjectileBase1 && ProjectileBase2 && ProjectileBase3)
	{
		ProjectileBase1->ActivateProjectile(GetOwner()->GetActorLocation(), Direction);
		ProjectileBase2->ActivateProjectile(GetOwner()->GetActorLocation(), LeftDirection);
		ProjectileBase3->ActivateProjectile(GetOwner()->GetActorLocation(), RightDirection);
	}
}

void UGenjiShurikenComponent::SetCooldown(float InCooldownTime)
{
	CooldownTime = InCooldownTime;
}

TObjectPtr<AOWProjectileBase> UGenjiShurikenComponent::GetProjectileFromPool()
{
	PoolIndex %= PoolSize + ExtraPoolSize;

	AOWProjectileBase* ProjectileBase = ProjectilePool[PoolIndex++];

	return ProjectileBase;
}

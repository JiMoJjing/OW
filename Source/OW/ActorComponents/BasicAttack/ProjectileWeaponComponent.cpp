// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeaponComponent.h"

#include "OW/Collision/OWCollisionProfile.h"
#include "OW/Interface/OWPlayerTraceInterface.h"
#include "OW/Projectile/OWProjectileBase.h"
#include "OW/Character/OWCharacterPlayable.h"

UProjectileWeaponComponent::UProjectileWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UProjectileWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(ProjectileClass)
	{
		UWorld* World = GetOwner()->GetWorld();
		FActorSpawnParameters ActorSpawnParameter;
		ActorSpawnParameter.Instigator = CharacterPlayable;
		ActorSpawnParameter.Owner = CharacterPlayable;
		
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

void UProjectileWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UProjectileWeaponComponent::Fire()
{
	if(CheckAmmo() == false)
	{
		return;
	}

	FVector ActorLocation = GetOwner()->GetActorLocation();
	FVector DirectionVector;
	PlayerTraceInterface->GetDirectionToCrosshair(ActorLocation, DirectionVector, OWTEAM1TRACE);

	if(AOWProjectileBase* ProjectileBase = GetProjectileFromPool())
	{
		ProjectileBase->ActivateProjectile(ActorLocation, DirectionVector);
		UseAmmo(1);
	}
}

TObjectPtr<AOWProjectileBase> UProjectileWeaponComponent::GetProjectileFromPool()
{
	if(ProjectilePool.IsEmpty())
	{
		return nullptr;
	}
	
	PoolIndex %= PoolSize + ExtraPoolSize;

	AOWProjectileBase* ProjectileBase = ProjectilePool[PoolIndex++];

	return ProjectileBase;
}

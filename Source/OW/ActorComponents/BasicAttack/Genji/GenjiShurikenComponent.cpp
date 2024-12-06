// Fill out your copyright notice in the Description page of Project Settings.


#include "GenjiShurikenComponent.h"

#include "OW/Character/OWCharacterPlayable.h"
#include "OW/Collision/OWCollisionProfile.h"
#include "OW/Projectile/OWProjectileBase.h"

UGenjiShurikenComponent::UGenjiShurikenComponent() : AltFireDelayTime(0.68f)
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BasicWeaponMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_PrimaryFire.AM_Genji_PrimaryFire'"));
	if(BasicWeaponMontageRef.Object)
	{
		BasicWeaponMontage = BasicWeaponMontageRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AltFireMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_SecondaryFire.AM_Genji_SecondaryFire'"));
	if(AltFireMontageRef.Object)
	{
		AltFireMontage = AltFireMontageRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_Reload.AM_Genji_Reload'"));
	if(ReloadMontageRef.Object)
	{
		ReloadMontage = ReloadMontageRef.Object;
	}
	
	
	static ConstructorHelpers::FClassFinder<AOWProjectileBase> ProjectileClassRef(TEXT("/Game/OW/Genji/Projectile/BP_Shuriken.BP_Shuriken_C"));
	if(ProjectileClassRef.Class)
	{
		ProjectileClass = ProjectileClassRef.Class;
	}

	DelayTime = 0.88f;
	
	PoolSize = 30;
	ExtraPoolSize = 30;
	PoolIndex = 0;
}

void UGenjiShurikenComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGenjiShurikenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UGenjiShurikenComponent::UseAltFire(float InPlayRate)
{
	if(CheckAmmo() == false)
	{
		return;
	}

	if(CheckAvailable())
	{
		CharacterPlayable->OnAnimNotify.AddUObject(this, &UGenjiShurikenComponent::AltFire);
		PlayAltFireMontage(InPlayRate);
		DelayStart(AltFireDelayTime);
	}
}

void UGenjiShurikenComponent::OnDragonbladeStart()
{
	Reload();
}

void UGenjiShurikenComponent::AltFire()
{
	if(CheckAmmo() == false)
	{
		return;
	}

	FVector ActorLocation = GetOwner()->GetActorLocation();
	FVector DirectionVector;
	PlayerTraceInterface->GetDirectionToCrosshair(ActorLocation, DirectionVector, OWTEAM1TRACE);
	const FVector LeftDirection = DirectionVector.RotateAngleAxis(-5.f, FVector::UpVector);
	const FVector RightDirection = DirectionVector.RotateAngleAxis(5.f, FVector::UpVector);

	AOWProjectileBase* ProjectileBase1 = GetProjectileFromPool();
	AOWProjectileBase* ProjectileBase2 = GetProjectileFromPool();
	AOWProjectileBase* ProjectileBase3 = GetProjectileFromPool();
	
	if(ProjectileBase1 && ProjectileBase2 && ProjectileBase3)
	{
		ProjectileBase1->ActivateProjectile(ActorLocation, DirectionVector);
		ProjectileBase2->ActivateProjectile(ActorLocation, LeftDirection);
		ProjectileBase3->ActivateProjectile(ActorLocation, RightDirection);
		UseAmmo(3);
	}
}

void UGenjiShurikenComponent::PlayAltFireMontage(float InPlayRate)
{
	if(AltFireMontage)
	{
		CharacterPlayable->GetMesh()->GetAnimInstance()->Montage_Play(AltFireMontage, InPlayRate);
	}
}

void UGenjiShurikenComponent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageBlendingOut(Montage, bInterrupted);
	
	if(Montage == AltFireMontage)
	{
		CharacterPlayable->OnAnimNotify.RemoveAll(this);
	}
	
}


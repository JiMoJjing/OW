// Fill out your copyright notice in the Description page of Project Settings.


#include "OWProjectileBase.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OW/Collision/OWCollisionProfile.h"


AOWProjectileBase::AOWProjectileBase() : ProjectileLifeSpan(5.f), ProjectileDamage(10.f)
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	HitSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphereComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));

	SetRootComponent(HitSphereComponent);
	StaticMeshComponent->SetupAttachment(HitSphereComponent);
	NiagaraComponent->SetupAttachment(HitSphereComponent);
	
	if(HitSphereComponent)
	{
		HitSphereComponent->OnComponentHit.AddDynamic(this, &AOWProjectileBase::OnSphereHit);
		HitSphereComponent->SetCollisionProfileName(OWTEAM1HIT);
		HitSphereComponent->CanCharacterStepUpOn = ECB_No;
	}

	if(StaticMeshComponent)
	{
		StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
		StaticMeshComponent->CanCharacterStepUpOn = ECB_No;
	}

	if(NiagaraComponent)
	{
		NiagaraComponent->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

void AOWProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	DeactivateProjectile();
}

void AOWProjectileBase::ActivateProjectile(const FVector& StartLocation, const FVector& Direction)
{
	SetActorLocation(StartLocation);
	SetActorRotation(Direction.Rotation());

	HitSphereComponent->Activate(true);
	HitSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	StaticMeshComponent->SetVisibility(true);

	NiagaraComponent->Activate(true);
	NiagaraComponent->SetVisibility(true);

	float Speed = ProjectileMovementComponent->InitialSpeed;
	ProjectileMovementComponent->Activate(true);
	ProjectileMovementComponent->Velocity = Direction * Speed;

	LifeSpanTimerStart();
}

void AOWProjectileBase::DeactivateProjectile()
{
	LifeSpanTimerEnd();
	
	ProjectileMovementComponent->Deactivate();
	
	HitSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitSphereComponent->Deactivate();

	StaticMeshComponent->SetVisibility(false);

	NiagaraComponent->SetVisibility(false);
	NiagaraComponent->Deactivate();	
}

void AOWProjectileBase::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const FVector HitLocation = GetActorLocation();

	if(OtherComp->GetCollisionObjectType() == ECC_WorldStatic)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitWallEffect, HitLocation, Hit.ImpactNormal.Rotation());
		DeactivateProjectile();
		return;
	}
	
	if(AActor* HitActor = OtherComp->GetOwner())
	{
		UGameplayStatics::ApplyPointDamage(HitActor, ProjectileDamage, HitLocation, Hit, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitActorEffect, HitLocation, Hit.ImpactNormal.Rotation());
	}
	
	DeactivateProjectile();
}

void AOWProjectileBase::LifeSpanTimerStart()
{
	GetWorldTimerManager().SetTimer(LifeSpanTimerHandle, this, &AOWProjectileBase::DeactivateProjectile, ProjectileLifeSpan, false);
}

void AOWProjectileBase::LifeSpanTimerEnd()
{
	GetWorldTimerManager().ClearTimer(LifeSpanTimerHandle);
}

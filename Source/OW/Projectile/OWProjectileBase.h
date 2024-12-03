// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWProjectileBase.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class OW_API AOWProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AOWProjectileBase();

protected:
	virtual void BeginPlay() override;

public:
	void ActivateProjectile(const FVector& StartLocation, const FVector& Direction);
	void DeactivateProjectile();

	void SetCollisionProfile(FName InCollisionProfileName);

protected:
	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void LifeSpanTimerStart();
	void LifeSpanTimerEnd();

	FTimerHandle LifeSpanTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ProjectileBase, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ProjectileBase, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> HitSphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ProjectileBase, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ProjectileBase, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ProjectileBase, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "10.0"))
	float ProjectileLifeSpan;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ProjectileBase, meta = (AllowPrivateAccess = "true"))
	float ProjectileDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ProjectileBase, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> HitActorEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ProjectileBase, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> HitWallEffect;
};

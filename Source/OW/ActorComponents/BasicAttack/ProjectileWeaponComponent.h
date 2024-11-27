// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/ActorComponents/BasicAttack/BasicWeaponComponent.h"
#include "ProjectileWeaponComponent.generated.h"


class AOWProjectileBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UProjectileWeaponComponent : public UBasicWeaponComponent
{
	GENERATED_BODY()

public:
	UProjectileWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void Fire() override;

	
// Projectile Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicWeapon_Projectile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOWProjectileBase> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = BasicWeapon_Projectile, meta = (AllowPrivateAccess = "true"))
	uint8 PoolSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = BasicWeapon_Projectile, meta = (AllowPrivateAccess = "true"))
	uint8 ExtraPoolSize;
	
	UPROPERTY()
	TArray<TObjectPtr<AOWProjectileBase>> ProjectilePool;
	
	UPROPERTY()
	uint8 PoolIndex;
	
	TObjectPtr<AOWProjectileBase> GetProjectileFromPool();
};

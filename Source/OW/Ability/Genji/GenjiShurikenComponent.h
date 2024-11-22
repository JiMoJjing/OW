// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/Ability/AbilityComponent.h"
#include "GenjiShurikenComponent.generated.h"


class AOWProjectileBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UGenjiShurikenComponent : public UAbilityComponent
{
	GENERATED_BODY()

public:
	UGenjiShurikenComponent();

protected:
	virtual void BeginPlay() override;

// Ability Function
private:
	virtual void AbilityStart() override;
	virtual void AbilityEnd() override;

	virtual void CooldownEnd() override;

// Secondary Function
public:
	void UseSecondAbility();

private:
	void SecondAbilityStart();

// Fire Function
	void PrimaryFire();
	void SecondaryFire();
	
	
// Cooldown
private:
	void SetCooldown(float InCooldownTime);
	
	UPROPERTY()
	float PrimaryFireCooldown;

	UPROPERTY()
	float SecondaryFireCooldown;

// Montage
	UPROPERTY()
	TObjectPtr<UAnimMontage> SecondAbilityMontage;

// Projectile
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOWProjectileBase> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = Projectile, meta = (AllowPrivateAccess = "true"))
	uint8 PoolSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = Projectile, meta = (AllowPrivateAccess = "true"))
	uint8 ExtraPoolSize;
	
	UPROPERTY()
	TArray<TObjectPtr<AOWProjectileBase>> ProjectilePool;

	UPROPERTY()
	uint8 PoolIndex;

	TObjectPtr<AOWProjectileBase> GetProjectileFromPool();
};

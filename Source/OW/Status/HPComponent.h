// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HPComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, float /* MaxHP */, float /* CurrentHP */);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UHPComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHPComponent();

protected:
	virtual void BeginPlay() override;

// Getter, Setter	
public:
	void SetMaxHP(float InMaxHP) { MaxHP = InMaxHP; }
	FORCEINLINE float GetMaxHP() { return MaxHP;}

// TakeDamage	
	float TakeDamage(float InDamage);

// Check Death
	bool IsDead();
	
// DELEGATE
	FOnHPChanged OnHPChanged;
private:
	void SetCurrentHP(float InHP);
	
	UPROPERTY()
	float MaxHP;

	UPROPERTY()
	float CurrentHP;
};

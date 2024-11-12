// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityState.h"
#include "AbilityType.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityStateChanged, EAbiltiyState /* AbilityState */)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityCooldownTimeChanged, float /* RemainingCooldownTime */)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();

protected:
	virtual void BeginPlay() override;

// Use Ability Section
public:
	void UseAbility();

protected:
	bool CanUseAbility();

	virtual void StartAbility();
	virtual void EndAbility();
	
// Ability Type Section
public:	
	FORCEINLINE EAbilityType GetAbilityType() const { return AbilityType; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_Type, meta = (AllowPrivateAccess = "true"))
	EAbilityType AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_Type, meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = EAbilityType))
	uint8 CancelableAbilityTypes = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_Type, meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = EAbilityType))
	uint8 UnavailableAbilityTypes = 0;

public:
	void OtherAbilityStarted(EAbilityType InAbilityType);
	void OtherAbilityEnded(EAbilityType InAbilityType);

// Ability State Section
public:
	FORCEINLINE EAbiltiyState GetAbilityState() const { return AbilityState; }
	
	void SetAbilityState(EAbiltiyState InAbilityState);

	FOnAbilityStateChanged OnAbilityStateChanged;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = Ability_State)
	EAbiltiyState AbilityState;

// Cooldown Section
public:
	FOnAbilityCooldownTimeChanged OnAbilityCooldownTimeChanged;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_Cooldown, meta = (AllowPrivateAccess = "true"))
	float CooldownTime;

	float RemainingCooldownTime;
	
	FTimerHandle CooldownTimerHandle;

	void CooldownTimerStart();
	void CooldownTimerEnd();
	void CooldownTimerTick();
	
// Montage Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_Montage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AbilityMontage;

	void PlayAbilityMontage();
};


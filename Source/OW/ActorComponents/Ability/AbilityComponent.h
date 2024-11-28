﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityState.h"
#include "AbilityType.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

class UAbilityManagerComponent;
class AOWCharacterPlayable;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityStateChanged, EAbilityState /* AbilityState */)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAbilityCooldownTimeChanged, float /* CooldownTime */,float /* CurrentCooldownTime */)


USTRUCT(BlueprintType)
struct FOnAbilityStateChangedDelegateWrapper
{
	GENERATED_BODY()

	FOnAbilityStateChangedDelegateWrapper() : OnAbilityStateChanged(nullptr) {}
	FOnAbilityStateChangedDelegateWrapper(FOnAbilityStateChanged& InDelegate) : OnAbilityStateChanged(&InDelegate) {}

	bool IsValid() const { return OnAbilityStateChanged != nullptr; }

	FOnAbilityStateChanged* OnAbilityStateChanged;
};

USTRUCT(BlueprintType)
struct FOnAbilityCooldownTimeChangedDelegateWrapper
{
	GENERATED_BODY()

	FOnAbilityCooldownTimeChangedDelegateWrapper() : OnAbilityCooldownTimeChanged(nullptr) {}
	FOnAbilityCooldownTimeChangedDelegateWrapper(FOnAbilityCooldownTimeChanged& InDelegate) : OnAbilityCooldownTimeChanged(&InDelegate) {}

	bool IsValid() const { return OnAbilityCooldownTimeChanged != nullptr; }

	FOnAbilityCooldownTimeChanged* OnAbilityCooldownTimeChanged;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();

	virtual void InitializeComponent() override;

protected:
	virtual void BeginPlay() override;

// Ability Function
public:
	void UseAbility();

protected:
	bool CanUseAbility();
	
	virtual void AbilityStart();
	virtual void AbilityEnd();


// Cooldown Section
	void CooldownStart();
	virtual void CooldownEnd();
	void CooldownTick();
	
	UPROPERTY()
	FTimerHandle CooldownTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_Cooldown, meta = (AllowPrivateAccess = "true"))
	float CooldownTime;

	UPROPERTY()
	float CurrentCooldownTime;

	
// AbilityType Section
	UPROPERTY()
	EAbilityType AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BitMask, BitMaskEnum = "/Script/OW.EAbilityType"))
	uint8 CancellableAbilityTypes = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BitMask, BitMaskEnum = "/Script/OW.EAbilityType"))
	uint8 MakeUnavailableAbilityTypes = 0;
	
	
// AbilityState Section
protected:
	UPROPERTY()
	EAbilityState AbilityState;
	
	void SetAbilityState(EAbilityState InAbilityState);


// With AbilityManagerComponent Section
protected:
	void OtherAbilityStart(EAbilityType OtherAbilityType);
	void OtherAbilityEnd(EAbilityType OtherAbilityType);

	
// Reference Caching Section
protected:
	UPROPERTY()
	TObjectPtr<AOWCharacterPlayable> CharacterPlayable;


// Montage Section	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_Montage, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AbilityMontage;

	void PlayAbilityMontage(UAnimMontage* InAbilityMontage);
	void PlayAbilityMontage_JumpToSection(UAnimMontage* InAbilityMontage, FName InSectionName);
	void StopAbilityMontage(float InBlendOutTime);

	
// 	Delegate Section
public:
	FOnAbilityCooldownTimeChanged OnAbilityCooldownTimeChanged;
	FOnAbilityStateChanged OnAbilityStateChanged;

// InitWidget
	void InitWidget();
};


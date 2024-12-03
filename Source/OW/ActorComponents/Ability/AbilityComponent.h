// Fill out your copyright notice in the Description page of Project Settings.

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

public:
	void UseAbility();

protected:
	bool CanUseAbility();
	
	virtual void AbilityStart();
	virtual void AbilityEnd();
	
	void CooldownStart();
	virtual void CooldownEnd();
	void CooldownTick();
	
	void SetAbilityState(EAbilityState InAbilityState);

	virtual void OtherAbilityStart(EAbilityType OtherAbilityType);
	virtual void OtherAbilityEnd(EAbilityType OtherAbilityType);

	void PlayAbilityMontage(UAnimMontage* InAbilityMontage);
	void PlayAbilityMontage_JumpToSection(UAnimMontage* InAbilityMontage, FName InSectionName);
	void StopAbilityMontage(float InBlendOutTime);

public:
	void InitWidget();

protected:
	FTimerHandle CooldownTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_Cooldown, meta = (AllowPrivateAccess = "true"))
	float CooldownTime;

	float CurrentCooldownTime;

	EAbilityType AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BitMask, BitMaskEnum = "/Script/OW.EAbilityType"))
	uint8 CancellableAbilityTypes = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BitMask, BitMaskEnum = "/Script/OW.EAbilityType"))
	uint8 MakeUnavailableAbilityTypes = 0;

	EAbilityState AbilityState;

	UPROPERTY()
	TObjectPtr<AOWCharacterPlayable> CharacterPlayable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_Montage, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AbilityMontage;

public:
	FOnAbilityCooldownTimeChanged OnAbilityCooldownTimeChanged;
	FOnAbilityStateChanged OnAbilityStateChanged;
};


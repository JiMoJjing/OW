// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWCharacterBase.h"
#include "OW/ActorComponents/Ability/AbilityType.h"
#include "OW/ActorComponents/Ability/AbilityComponent.h"
#include "OW/Interface/OWTriggerAnimNotifyInterface.h"
#include "OW/Interface/OWApplyDamageInterface.h"
#include "OW/Interface/OWPlayerTraceInterface.h"
#include "OWCharacterPlayable.generated.h"


class UQuickMeleeComponent;
class UBasicWeaponComponent;
class UAbilityManagerComponent;
class UAbilityComponent;
class UCameraComponent;
class USpringArmComponent;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityStart, EAbilityType /* InAbilityType */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityEnd, EAbilityType /* InAbilityType */);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUltimateGaugeChanged, float /* MaxUltimateGauge */, float /* CurrentUltimateGauge */);

/**
 * 
 */
UCLASS()
class OW_API AOWCharacterPlayable : public AOWCharacterBase, public IOWApplyDamageInterface, public IOWPlayerTraceInterface
	, public IOWTriggerAnimNotifyInterface
{
	GENERATED_BODY()

public:
	AOWCharacterPlayable();

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	void SetIgnoreInput(bool bIgnore);

	virtual void ApplyDamageSuccess(float Damage, bool bIsHeadShot) override;
	virtual void KillSuccess() override;

	virtual bool TraceUnderCrosshair(const float TraceDistance, FHitResult& OutHitResult, FVector& OutHitLocation, const ECollisionChannel InCollisionChannel) override;
	virtual void GetDirectionToCrosshair(const FVector& StartLocation, FVector& OutDirection, const ECollisionChannel InCollisionChannel) override;

	virtual void TriggerAnimNotify() override;
	virtual void TriggerAnimNotifyBegin() override;
	virtual void TriggerAnimNotifyEnd() override;
	virtual void TriggerAnimNotifyState(float DeltaTime) override;
	
	UBasicWeaponComponent* GetBasicWeaponComponent() { return BasicWeaponComponent; }
	UQuickMeleeComponent* GetQuickMeleeComponent() { return QuickMeleeComponent; }
	FORCEINLINE bool GetQuickMeleeActive() const { return bQuickMeleeActive; }
	void SetQuickMeleeActive(bool bActive);
	
	void AbilityStart(EAbilityType InAbilityType);
	void AbilityEnd(EAbilityType InAbilityType);
	
	FORCEINLINE EAbilityType GetCurrentAbilityType() const { return CurrentAbilityType; }
	
	void AddAbilityStateChangedDelegate(const EAbilityType InAbilityType, const FOnAbilityStateChangedDelegateWrapper& InDelegateWrapper);
	void AddAbilityCooldownTimeChangedDelegate(const EAbilityType InAbilityType, const FOnAbilityCooldownTimeChangedDelegateWrapper& InDelegateWrapper);

	FOnAbilityStateChangedDelegateWrapper& GetAbilityStateChangedDelegateWrapper(EAbilityType InAbilityType) { return AbilityStateChangedDelegateWrappers[InAbilityType]; }
	FOnAbilityCooldownTimeChangedDelegateWrapper& GetAbilityCooldownTimeChangedDelegateWrapper(EAbilityType InAbilityType) { return AbilityCooldownTimeChangedDelegateWrappers[InAbilityType]; }

protected:
	void AddUltimateGauge(float InAmount);
	void AutoAddUltimateGaugeTimerStart();
	void AutoAddUltimateGauge();

public:
	void SetUltimateActive(bool bActive);
	void UltimateUsed();

	FORCEINLINE bool IsUltimateGaugeFull() const { return FMath::IsNearlyEqual(CurrentUltimateGauge, MaxUltimateGauge); }
	FORCEINLINE bool IsUltimateActive() const { return bUltimateActive; }

	virtual void InitWidget();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> UpperArmLCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> LowerArmLCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> UpperArmRCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> LowerArmRCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> ThighLCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CalfLCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> FootLCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> ThighRCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CalfRCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> FootRCollision;

	UPROPERTY()
	TObjectPtr<APlayerController> OwnerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicWeapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBasicWeaponComponent> BasicWeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuickMelee, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UQuickMeleeComponent> QuickMeleeComponent;

	uint8 bQuickMeleeActive : 1;

	EAbilityType CurrentAbilityType;

	UPROPERTY()
	TMap<EAbilityType, FOnAbilityStateChangedDelegateWrapper> AbilityStateChangedDelegateWrappers;

	UPROPERTY()
	TMap<EAbilityType, FOnAbilityCooldownTimeChangedDelegateWrapper> AbilityCooldownTimeChangedDelegateWrappers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_Ultimate, meta = (AllowPrivateAccess = "true"))
	float MaxUltimateGauge;

	UPROPERTY()
	float CurrentUltimateGauge;
	
	uint8 bUltimateActive : 1;

	FTimerHandle AutoAddUltimateGaugeTimerHandle;

	uint8 bIsJumpPressed : 1;
	
public:
	FOnAnimNotify OnAnimNotify;
	FOnAnimNotifyBegin OnAnimNotifyBegin;
	FOnAnimNotifyEnd OnAnimNotifyEnd;
	FOnAnimNotifyState OnAnimNotifyState;
	
	FOnAbilityStart OnAbilityStart;
	FOnAbilityEnd OnAbilityEnd;
	
	FOnUltimateGaugeChanged OnUltimateGaugeChanged;
};

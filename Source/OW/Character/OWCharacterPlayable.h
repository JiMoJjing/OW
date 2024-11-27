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


class UBasicWeaponComponent;
class UAbilityManagerComponent;
class UAbilityComponent;
class UCameraComponent;
class USpringArmComponent;
 
DECLARE_MULTICAST_DELEGATE(FOnAnimNotify);
DECLARE_MULTICAST_DELEGATE(FOnAnimNotifyBegin);
DECLARE_MULTICAST_DELEGATE(FOnAnimNotifyEnd);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAnimNotifyState, float /* DeltaTime */);

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

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	
// View Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	
// Input Section
public:
	void SetIgnoreInput(bool bIgnore);
	

// AbilityComponent Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TMap<EAbilityType, TObjectPtr<UAbilityComponent>> AbilityComponents;

// BasicWeaponComponent Section
	UPROPERTY()
	TObjectPtr<UBasicWeaponComponent> BasicWeaponComponent;

public:
	UBasicWeaponComponent* GetBasicWeaponComponent() { return BasicWeaponComponent; }

	
// Collision Section
protected:
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

	
// Controller Reference Caching
protected:
	UPROPERTY()
	TObjectPtr<APlayerController> OwnerController;


// Ability Delegate Wrappers
protected:
	UPROPERTY()
	TMap<EAbilityType, FOnAbilityStateChangedDelegateWrapper> AbilityStateChangedDelegateWrappers;

	UPROPERTY()
	TMap<EAbilityType, FOnAbilityCooldownTimeChangedDelegateWrapper> AbilityCooldownTimeChangedDelegateWrappers;

public:
	void AddAbilityStateChangedDelegate(const EAbilityType InAbilityType, const FOnAbilityStateChangedDelegateWrapper& InDelegateWrapper);
	void AddAbilityCooldownTimeChangedDelegate(const EAbilityType InAbilityType, const FOnAbilityCooldownTimeChangedDelegateWrapper& InDelegateWrapper);

	FOnAbilityStateChangedDelegateWrapper& GetAbilityStateChangedDelegateWrapper(EAbilityType InAbilityType) { return AbilityStateChangedDelegateWrappers[InAbilityType]; }
	FOnAbilityCooldownTimeChangedDelegateWrapper& GetAbilityCooldownTimeChangedDelegateWrapper(EAbilityType InAbilityType) { return AbilityCooldownTimeChangedDelegateWrappers[InAbilityType]; }

	
// IOWApplyDamageInterface
public:
	virtual void ApplyDamageSuccess(float Damage, bool bIsHeadShot) override;
	virtual void KillSuccess() override;

	
// IOWPlayerTraceInterface
	virtual bool TraceUnderCrosshair(const float TraceDistance, FHitResult& OutHitResult, FVector& OutHitLocation, const ECollisionChannel InCollisionChannel) override;
	virtual void GetDirectionToCrosshair(const FVector& StartLocation, FVector& OutDirection, const ECollisionChannel InCollisionChannel) override;


// AnimNotify Trigger DELEGATE
	FOnAnimNotify OnAnimNotify;
	FOnAnimNotifyBegin OnAnimNotifyBegin;
	FOnAnimNotifyEnd OnAnimNotifyEnd;
	FOnAnimNotifyState OnAnimNotifyState;


// IOWTriggerAnimNotifyInterface
	virtual void TriggerAnimNotify() override;
	virtual void TriggerAnimNotifyBegin() override;
	virtual void TriggerAnimNotifyEnd() override;
	virtual void TriggerAnimNotifyState(float DeltaTime) override;

	
// Widget
	virtual void InitWidget();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWCharacterBase.h"
#include "OW/Ability/AbilityType.h"
#include "OW/Interface/OWTriggerAnimNotifyInterface.h"
#include "OW/Interface/OWApplyDamageInterface.h"
#include "OW/Interface/OWCharacterInputInterface.h"
#include "OW/Interface/OWPlayerTraceInterface.h"
#include "OWCharacterPlayable.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAnimNotifyBegin);
DECLARE_MULTICAST_DELEGATE(FOnAnimNotifyEnd);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAnimNotifyState, float /* DeltaTime */);


class UAbilityManagerComponent;
class UAbilityComponent;
class UCameraComponent;
class USpringArmComponent;

/**
 * 
 */
UCLASS()
class OW_API AOWCharacterPlayable : public AOWCharacterBase, public IOWCharacterInputInterface, public IOWApplyDamageInterface, public IOWPlayerTraceInterface
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
	virtual void PrimaryFire() override;
	virtual void SecondaryFire() override;
	virtual void AbilityOne() override;
	virtual void AbilityTwo() override;
	virtual void AbilityThree() override;
	virtual void Reload() override;
	virtual void QuickMelee() override;

// AbilityComponent Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityManagerComponent> AbilityManagerComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> PrimaryFireComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> SecondaryFireComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> AbilityOneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> AbilityTwoComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> AbilityThreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> ReloadComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> QuickMeleeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TMap<EAbilityType, TObjectPtr<UAbilityComponent>> AbilityComponents;

public:
	FORCEINLINE UAbilityManagerComponent* GetAbilityManagerComponent() { return AbilityManagerComponent; }

	void SetIgnoreInput(bool bIgnore);
	
// Controller Reference Caching
protected:
	UPROPERTY()
	TObjectPtr<APlayerController> OwnerController;
	

// IOWApplyDamageInterface
public:
	virtual void ApplyDamageSuccess(float Damage, bool bIsHeadShot) override;
	virtual void KillSuccess() override;

// IOWPlayerTraceInterface
	virtual bool TraceUnderCrosshair(const float TraceDistance, FHitResult& OutHitResult, FVector& OutHitLocation, const ECollisionChannel InCollisionChannel) override;
	virtual void GetDirectionToCrosshair(const FVector& StartLocation, FVector& OutDirection, const ECollisionChannel InCollisionChannel) override;



// AnimNotify
	FOnAnimNotifyBegin OnAnimNotifyBegin;
	FOnAnimNotifyEnd OnAnimNotifyEnd;
	FOnAnimNotifyState OnAnimNotifyState;

	virtual void TriggerAnimNotifyBegin() override;
	virtual void TriggerAnimNotifyEnd() override;
	virtual void TriggerAnimNotifyState(float DeltaTime) override;
};

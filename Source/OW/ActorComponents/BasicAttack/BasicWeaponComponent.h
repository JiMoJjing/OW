// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasicWeaponComponent.generated.h"


class IOWTriggerAnimNotifyInterface;
class IOWPlayerTraceInterface;
class AOWCharacterPlayable;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, uint8 /* MaxAmmo */, uint8 /* CurrentAmmo */);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UBasicWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBasicWeaponComponent();

	virtual void InitializeComponent() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Use Function
public:
	void UseBasicWeapon(float InPlayRate);

protected:
	bool CheckAvailable();

	virtual void Fire();
	
// Caching Section	
protected:
	UPROPERTY()
	TObjectPtr<AOWCharacterPlayable> CharacterPlayable;

	UPROPERTY()
	TScriptInterface<IOWPlayerTraceInterface> PlayerTraceInterface;

	UPROPERTY()
	TScriptInterface<IOWTriggerAnimNotifyInterface> TriggerAnimNotifyInterface;

	
// AnimMontage Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicWeapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> BasicWeaponMontage;

	void PlayBasicWeaponMontage(float InPlayRate);

	UFUNCTION()
	virtual void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	
// Delay Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicWeapon, meta = (AllowPrivateAccess = "true"))
	float DelayTime;
	
	UPROPERTY()
	FTimerHandle DelayTimerHandle;

	UPROPERTY()
	uint8 bDelayActive : 1;

	void DelayStart(float InDelayTime);
	void DelayEnd();
	

// Ammo Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicWeapon, meta = (AllowPrivateAccess = "true"))
	uint8 MaxAmmo;

	UPROPERTY()
	uint8 CurrentAmmo;

	bool CheckAmmo();

	void UseAmmo(uint8 InAmount);

	void AmmoChanged();
	
public:
	FOnAmmoChanged OnAmmoChanged;

	void InitWidget();


// Reload Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicWeapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ReloadMontage;

	void PlayReloadMontage(float InPlayRate);

	void Reload();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicWeapon, meta = (AllowPrivateAccess = "true"))
	float ReloadDelayTime;
	
public:
	void UseReload(float InPlayRate);
	
};

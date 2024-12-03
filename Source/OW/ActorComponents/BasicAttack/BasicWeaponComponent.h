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

	void UseBasicWeapon(float InPlayRate);

protected:
	bool CheckAvailable();

	virtual void Fire();

	void PlayBasicWeaponMontage(float InPlayRate);

	UFUNCTION()
	virtual void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	void DelayStart(float InDelayTime);
	void DelayEnd();

	bool CheckAmmo();
	void UseAmmo(uint8 InAmount);
	void AmmoChanged();

	void PlayReloadMontage(float InPlayRate);
	void Reload();

public:
	void InitWidget();
	void UseReload(float InPlayRate);

protected:
	UPROPERTY()
	TObjectPtr<AOWCharacterPlayable> CharacterPlayable;

	UPROPERTY()
	TScriptInterface<IOWPlayerTraceInterface> PlayerTraceInterface;

	UPROPERTY()
	TScriptInterface<IOWTriggerAnimNotifyInterface> TriggerAnimNotifyInterface;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicWeapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> BasicWeaponMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicWeapon, meta = (AllowPrivateAccess = "true"))
	float DelayTime;

	FTimerHandle DelayTimerHandle;

	uint8 bDelayActive : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicWeapon, meta = (AllowPrivateAccess = "true"))
	uint8 MaxAmmo;
	
	uint8 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicWeapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ReloadMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicWeapon, meta = (AllowPrivateAccess = "true"))
	float ReloadDelayTime;
	
public:
	FOnAmmoChanged OnAmmoChanged;
};

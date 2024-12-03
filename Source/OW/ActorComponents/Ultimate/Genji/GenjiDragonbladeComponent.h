// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/ActorComponents/Ultimate/UltimateAbilityBaseComponent.h"
#include "GenjiDragonbladeComponent.generated.h"


class AOWGenji;
class UNiagaraSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UGenjiDragonbladeComponent : public UUltimateAbilityBaseComponent
{
	GENERATED_BODY()

public:
	UGenjiDragonbladeComponent();

	virtual void InitializeComponent() override;

protected:
	virtual void BeginPlay() override;

	virtual void StartUltimate() override;
	virtual void EndUltimate() override;

// Cache Genji	
private:
	UPROPERTY()
	TObjectPtr<AOWGenji> GenjiRef;
	
// Dragonblade Duration	
	FTimerHandle DragonbladeDurationTimerHandle;

	void DragonbladeDurationStart();
	void DragonbladeDurationEnd();

	UFUNCTION()
	void DragonbladeDurationDelayEnd(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dragonblade, meta = (AllowPrivateAccess = "true"))
	float DragonbladeDuration;

// Dragonblade To Normal	
	void DragonbladeToNormal();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dragonblade, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DragonbladeToNormalMontage;


// Dragonblade Slash	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dragonblade, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DragonbladeSlashMontage;

	UPROPERTY()
	uint8 DragonbladeSlashIndex;

	TMap<uint8, FName> DragonbladeSlashSections;

	void DragonbladeSlashTrace();

// Dragonblade Slash Delay	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dragonblade, meta = (AllowPrivateAccess = "true"))
	float DragonbladeSlashDelayTime;
	
	UPROPERTY()
	uint8 bDragonbladeSlashDelay : 1;

	FTimerHandle DragonbladeSlashDelayTimerHandle;
	
	void DragonbladeSlashDelayStart();
	void DragonbladeSlashDelayEnd();

// Niagara
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dragonblade, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> DragonbladeSlashHitEffect;
	

// For Reference Function	
public:
	void DragonbladeSlash();
};
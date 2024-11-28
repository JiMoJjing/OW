// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuickMeleeComponent.generated.h"


class UBoxComponent;
class IOWTriggerAnimNotifyInterface;
class AOWCharacterPlayable;
class UNiagaraSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UQuickMeleeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuickMeleeComponent();

	virtual void InitializeComponent() override;

protected:
	virtual void BeginPlay() override;

public:
	void UseQuickMelee(float InPlayRate);

protected:
	bool CheckAvailable();

// Chaching	
	UPROPERTY()
	TObjectPtr<AOWCharacterPlayable> CharacterPlayable;

	UPROPERTY()
	TScriptInterface<IOWTriggerAnimNotifyInterface> TriggerAnimNotifyInterface;

// Montage	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuickMelee, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> QuickMeleeMontage;

	void PlayQuickMeleeMontage(float InPlayRate);

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

// Delay
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuickMelee, meta = (AllowPrivateAccess = "true"))
	float DelayTime;

	UPROPERTY()
	FTimerHandle DelayTimerHandle;
	
	UPROPERTY()
	uint8 bDelayActive : 1;

	void DelayStart(float InDelayTime);
	void DelayEnd();

// Collision
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuickMelee, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> QuickMeleeBoxComponent;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TArray<TObjectPtr<AActor>> OverlappedActors;

	void QuickMeleeBegin();
	void QuickMeleeEnd();

// Damage	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuickMelee, meta = (AllowPrivateAccess = "true"))
	float QuickMeleeDamage;
	
// Niagara
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = QuickMelee, meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraSystem> QuickMeleeEffect;
};

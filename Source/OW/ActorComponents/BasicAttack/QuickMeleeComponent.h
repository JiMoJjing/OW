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
	void PlayQuickMeleeMontage(float InPlayRate);

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	void DelayStart(float InDelayTime);
	void DelayEnd();

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void QuickMeleeBegin();
	void QuickMeleeEnd();

	UPROPERTY()
	TObjectPtr<AOWCharacterPlayable> CharacterPlayable;

	UPROPERTY()
	TScriptInterface<IOWTriggerAnimNotifyInterface> TriggerAnimNotifyInterface;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuickMelee, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> QuickMeleeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuickMelee, meta = (AllowPrivateAccess = "true"))
	float DelayTime;

	FTimerHandle DelayTimerHandle;
	
	uint8 bDelayActive : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuickMelee, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> QuickMeleeBoxComponent;

	TArray<TObjectPtr<AActor>> OverlappedActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuickMelee, meta = (AllowPrivateAccess = "true"))
	float QuickMeleeDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = QuickMelee, meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraSystem> QuickMeleeEffect;
};

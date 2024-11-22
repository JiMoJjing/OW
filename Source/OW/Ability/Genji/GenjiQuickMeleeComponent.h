// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/Ability/AbilityComponent.h"
#include "GenjiQuickMeleeComponent.generated.h"


class UBoxComponent;
class UNiagaraSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UGenjiQuickMeleeComponent : public UAbilityComponent
{
	GENERATED_BODY()

public:
	UGenjiQuickMeleeComponent();

protected:
	virtual void BeginPlay() override;

// Ability Function
private:
	virtual void AbilityStart() override;
	virtual void AbilityEnd() override;

	void QuickMeleeBegin();
	void QuickMeleeEnd();

	
// Collider	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability_QuickMelee, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> QuickMeleeCollider;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	TArray<TObjectPtr<AActor>> OverlappedActors;

	
// Damage	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_QuickMelee, meta = (AllowPrivateAccess = "true"))
	float QuickMeleeDamage;

	
// Niagara
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability_SwiftStrike, meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraSystem> QuickMeleeEffect;
};

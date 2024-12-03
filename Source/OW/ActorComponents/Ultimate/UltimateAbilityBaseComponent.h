// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OW/ActorComponents/Ability/AbilityType.h"
#include "UltimateAbilityBaseComponent.generated.h"


class AOWCharacterPlayable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UUltimateAbilityBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUltimateAbilityBaseComponent();

	virtual void InitializeComponent() override;

protected:
	virtual void BeginPlay() override;

// Caching	
	UPROPERTY()
	TObjectPtr<AOWCharacterPlayable> CharacterPlayable;

public:
	void UseUltimate();

protected:
	virtual bool CheckAvailable();

	virtual void StartUltimate();
	virtual void EndUltimate();

	UPROPERTY()
	EAbilityType AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BitMask, BitMaskEnum = "/Script/OW.EAbilityType"))
	uint8 CancellableAbilityTypes = 0;

	// Montage	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuickMelee, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> UltimateMontage;

	void PlayUltimateMontage(UAnimMontage* Montage);

};

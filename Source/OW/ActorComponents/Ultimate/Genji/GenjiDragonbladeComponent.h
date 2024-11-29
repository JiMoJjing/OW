// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/ActorComponents/Ultimate/UltimateAbilityBaseComponent.h"
#include "GenjiDragonbladeComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UGenjiDragonbladeComponent : public UUltimateAbilityBaseComponent
{
	GENERATED_BODY()

public:
	UGenjiDragonbladeComponent();

protected:
	virtual void BeginPlay() override;

	virtual void StartUltimate() override;
	virtual void EndUltimate() override;

private:
	void DragonbladeModeStart();

	FTimerHandle DragonbladeDurationTimerHandle;

	void DragonbladeDurationStart();
	void DragonbladeDurationEnd();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dragonblade, meta = (AllowPrivateAccess = "true"))
	float DragonbladeDuration;

};

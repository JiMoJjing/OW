// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/Character/OWCharacterBase.h"
#include "OWStandardBot.generated.h"

UCLASS()
class OW_API AOWStandardBot : public AOWCharacterBase
{
	GENERATED_BODY()

public:
	AOWStandardBot();

protected:
	virtual void BeginPlay() override;

	virtual void CharacterDeath() override;

	virtual void CharacterRevive() override;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> ReviveAnimMontage;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWAnimInstance.h"
#include "GenjiAnimInstance.generated.h"

class AOWGenji;
/**
 * 
 */
UCLASS()
class OW_API UGenjiAnimInstance : public UOWAnimInstance
{
	GENERATED_BODY()

public:
	UGenjiAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<AOWGenji> GenjiRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bDragonbladeActive : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsClimbWall : 1;
};

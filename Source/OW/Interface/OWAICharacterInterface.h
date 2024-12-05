// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OWAICharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UOWAICharacterInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAICharacterAttackFinished);

/**
 * 
 */
class OW_API IOWAICharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITrunSpeed() = 0;
	virtual FVector GetAIPatrolPos() = 0;
	virtual ECollisionChannel GetAITraceChannel() = 0;
	
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) = 0;
	virtual void AIAttack() = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OWTriggerAnimNotifyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOWTriggerAnimNotifyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OW_API IOWTriggerAnimNotifyInterface
{
	GENERATED_BODY()

public:
	virtual void TriggerAnimNotifyBegin() = 0;
	virtual void TriggerAnimNotifyEnd() = 0;
	virtual void TriggerAnimNotifyState(float Delta) = 0;
};

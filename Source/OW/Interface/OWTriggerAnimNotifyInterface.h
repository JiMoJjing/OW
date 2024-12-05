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

DECLARE_MULTICAST_DELEGATE(FOnAnimNotify);
DECLARE_MULTICAST_DELEGATE(FOnAnimNotifyBegin);
DECLARE_MULTICAST_DELEGATE(FOnAnimNotifyEnd);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAnimNotifyState, float /* DeltaTime */);

/**
 * 
 */
class OW_API IOWTriggerAnimNotifyInterface
{
	GENERATED_BODY()

public:
	virtual void TriggerAnimNotify() = 0;
	virtual void TriggerAnimNotifyBegin() = 0;
	virtual void TriggerAnimNotifyEnd() = 0;
	virtual void TriggerAnimNotifyState(float Delta) = 0;
};

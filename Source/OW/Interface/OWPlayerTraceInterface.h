// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OWPlayerTraceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UOWPlayerTraceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OW_API IOWPlayerTraceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool TraceUnderCrosshair(const float TraceDistance, FHitResult& OutHitResult, FVector& OutHitLocation, const ECollisionChannel InCollisionChannel) = 0;
	virtual void GetDirectionToCrosshair(const FVector& StartLocation, FVector& OutDirection, const ECollisionChannel InCollisionChannel) = 0;
};

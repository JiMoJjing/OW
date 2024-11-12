// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityType.h"
#include "Components/ActorComponent.h"
#include "AbilityManagerComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityStarted, EAbilityType/* AbilityType */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityEnded, EAbilityType/* AbilityType */);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UAbilityManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityManagerComponent();

protected:
	virtual void BeginPlay() override;


// AbilityType Section
public:
	FORCEINLINE EAbilityType GetCurrentAbilityType() const { return CurrentAbilityType; }
private:
	EAbilityType CurrentAbilityType;

// DELEGATE Section
public:
	FOnAbilityStarted OnAbilityStarted;
	FOnAbilityEnded OnAbilityEnded;

	void AbilityStarted(EAbilityType InAbilityType);
	void AbilityEnded(EAbilityType InAbilityType);
	
};

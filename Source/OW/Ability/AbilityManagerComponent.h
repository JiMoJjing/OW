// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityType.h"
#include "Components/ActorComponent.h"
#include "AbilityManagerComponent.generated.h"

USTRUCT(BlueprintType)
struct FAbilitySettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BitMask, BitMaskEnum = "/Script/OW.EAbilityType"))
	uint8 CancellableAbilityTypes = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BitMask, BitMaskEnum = "/Script/OW.EAbilityType"))
	uint8 MakeUnavailableAbilityTypes = 0;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOtherAbilityStart, EAbilityType/* AbilityType */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOtherAbilityEnd, EAbilityType/* AbilityType */);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UAbilityManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityManagerComponent();

protected:
	virtual void BeginPlay() override;

	
// UseAbility Check
public:
	bool CanUseAbility(EAbilityType InAbilityType);
	
	void AbilityStart(EAbilityType InAbilityType);
	void AbilityEnd(EAbilityType InAbilityType);

	
// AbilityType Section
public:
	FORCEINLINE EAbilityType GetCurrentAbilityType() const { return CurrentAbilityType; }
	
private:
	EAbilityType CurrentAbilityType;


// AbilitySettings
public:
	uint8 GetMakeUnavailableAbilityTypes(EAbilityType InAbilityType) { return AbilitySettings[InAbilityType].MakeUnavailableAbilityTypes; }
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_Settings", meta = (AllowPrivateAccess = "true"))
	TMap<EAbilityType, FAbilitySettings> AbilitySettings;

	
// DELEGATE Section
public:
	FOnOtherAbilityStart OnOtherAbilityStart;
	FOnOtherAbilityEnd OnOtherAbilityEnd;

private:
	void OtherAbilityStart(EAbilityType InAbilityType);
	void OtherAbilityEnd(EAbilityType InAbilityType);
	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityManagerComponent.h"


UAbilityManagerComponent::UAbilityManagerComponent() : CurrentAbilityType(EAbilityType::EAT_None)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilityManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UAbilityManagerComponent::CanUseAbility(EAbilityType InAbilityType)
{
	if(InAbilityType == EAbilityType::EAT_None)
	{
		return false;
	}
	
	if(CurrentAbilityType == EAbilityType::EAT_None)
	{
		return true;
	}

	bool bCanUseAbility = AbilitySettings[InAbilityType].CancellableAbilityTypes & static_cast<uint8>(CurrentAbilityType);
	
	return bCanUseAbility;
}

void UAbilityManagerComponent::AbilityStart(EAbilityType InAbilityType)
{
	CurrentAbilityType = InAbilityType;
	OtherAbilityStart(CurrentAbilityType);
}

void UAbilityManagerComponent::AbilityEnd(EAbilityType InAbilityType)
{
	if(CurrentAbilityType == InAbilityType)
	{
		CurrentAbilityType = EAbilityType::EAT_None;
	}
	OtherAbilityEnd(InAbilityType);
}

uint8 UAbilityManagerComponent::GetMakeUnavailableAbilityTypes(EAbilityType InAbilityType)
{
	if(AbilitySettings.Contains(InAbilityType))
	{
		return AbilitySettings[InAbilityType].MakeUnavailableAbilityTypes;
	}
	return 0;
}

void UAbilityManagerComponent::OtherAbilityStart(EAbilityType InAbilityType)
{
	if(OnOtherAbilityStart.IsBound())
	{
		OnOtherAbilityStart.Broadcast(InAbilityType);
	}
}

void UAbilityManagerComponent::OtherAbilityEnd(EAbilityType InAbilityType)
{
	if(OnOtherAbilityEnd.IsBound())
	{
		OnOtherAbilityEnd.Broadcast(InAbilityType);
	}
}

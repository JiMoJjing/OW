// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityManagerComponent.h"


UAbilityManagerComponent::UAbilityManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentAbilityType = EAbilityType::EAT_None;
}

void UAbilityManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAbilityManagerComponent::AbilityStarted(EAbilityType InAbilityType)
{
	CurrentAbilityType = InAbilityType;
	
	if(OnAbilityStarted.IsBound())
	{
		OnAbilityStarted.Broadcast(InAbilityType);
	}
}

void UAbilityManagerComponent::AbilityEnded(EAbilityType InAbilityType)
{
	if(CurrentAbilityType == InAbilityType)
	{
		CurrentAbilityType = EAbilityType::EAT_None;
	}
	
	if(OnAbilityEnded.IsBound())
	{
		OnAbilityEnded.Broadcast(InAbilityType);
	}
}

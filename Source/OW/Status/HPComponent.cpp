// Fill out your copyright notice in the Description page of Project Settings.


#include "HPComponent.h"


UHPComponent::UHPComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHP = 200.f;
}

void UHPComponent::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentHP(MaxHP);
}

float UHPComponent::TakeDamage(float InDamage)
{
	float ActualDamage = InDamage;
	
	if(CurrentHP < ActualDamage)
	{
		ActualDamage = CurrentHP;
		SetCurrentHP(0.0f);

		return ActualDamage;
	}

	SetCurrentHP(CurrentHP - ActualDamage);
	return ActualDamage;
}

void UHPComponent::InitializeWidget()
{
	SetCurrentHP(MaxHP);
}

bool UHPComponent::IsDead()
{
	return CurrentHP <= 0.0f;
}

void UHPComponent::SetCurrentHP(float InHP)
{
	CurrentHP = FMath::Clamp(InHP, 0.0f, MaxHP);
	
	if(OnHPChanged.IsBound())
	{
		OnHPChanged.Broadcast(MaxHP, CurrentHP);
	}
}

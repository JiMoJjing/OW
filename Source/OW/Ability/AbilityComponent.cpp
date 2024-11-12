// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"

#include "GameFramework/Character.h"


UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAbilityComponent::UseAbility()
{
	if(CanUseAbility())
	{
		StartAbility();
	}
}

bool UAbilityComponent::CanUseAbility()
{
	return true;
}

void UAbilityComponent::StartAbility()
{
}

void UAbilityComponent::EndAbility()
{
}

void UAbilityComponent::OtherAbilityStarted(EAbilityType InAbilityType)
{
	if(UnavailableAbilityTypes & static_cast<uint8>(InAbilityType))
	{
		SetAbilityState(EAbiltiyState::EAS_Unavailable);
	}
}

void UAbilityComponent::OtherAbilityEnded(EAbilityType InAbilityType)
{
	if(UnavailableAbilityTypes & static_cast<uint8>(InAbilityType))
	{
		bool bIsCooldown = GetWorld()->GetTimerManager().IsTimerActive(CooldownTimerHandle);
		bIsCooldown ? SetAbilityState(EAbiltiyState::EAS_Cooldown) : SetAbilityState(EAbiltiyState::EAS_Active);
	}
}

void UAbilityComponent::SetAbilityState(EAbiltiyState InAbilityState)
{
	AbilityState = InAbilityState;
	if(OnAbilityStateChanged.IsBound())
	{
		OnAbilityStateChanged.Broadcast(AbilityState);
	}
}


void UAbilityComponent::CooldownTimerStart()
{
	SetAbilityState(EAbiltiyState::EAS_Cooldown);
	
	if(OnAbilityCooldownTimeChanged.IsBound())
	{
		RemainingCooldownTime = CooldownTime;
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UAbilityComponent::CooldownTimerTick, 0.1f, true, 0.1f);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UAbilityComponent::CooldownTimerEnd, CooldownTime, false);
	}
}

void UAbilityComponent::CooldownTimerEnd()
{
	if(AbilityState != EAbiltiyState::EAS_Unavailable)
	{
		SetAbilityState(EAbiltiyState::EAS_Active);
	}
}

void UAbilityComponent::CooldownTimerTick()
{
	RemainingCooldownTime = FMath::Clamp(RemainingCooldownTime - 0.1f, 0.f, CooldownTime);
	OnAbilityCooldownTimeChanged.Broadcast(RemainingCooldownTime);

	if(RemainingCooldownTime <= KINDA_SMALL_NUMBER)
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
		CooldownTimerEnd();
	}
}

void UAbilityComponent::PlayAbilityMontage()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	if(OwnerCharacter && AbilityMontage)
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(AbilityMontage);
	}
}

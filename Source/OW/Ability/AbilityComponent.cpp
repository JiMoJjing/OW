// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"

#include "OW/Ability/AbilityManagerComponent.h"
#include "OW/Character/OWCharacterPlayable.h"


UAbilityComponent::UAbilityComponent() : CooldownTime(0.f), CurrentCooldownTime(0.f), AbilityType(EAbilityType::EAT_None), MakeUnavailableAbilityType(0)
, AbilityState(EAbilityState::EAS_Available), PlayableCharacter(nullptr), AbilityManagerComponent(nullptr), AbilityMontage(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	if(AOWCharacterPlayable* OwnerCharacter = Cast<AOWCharacterPlayable>(GetOwner()))
	{
		PlayableCharacter = OwnerCharacter;
	}

	if(PlayableCharacter)
	{
		AbilityManagerComponent = PlayableCharacter->GetAbilityManagerComponent();
	}

	if(AbilityManagerComponent)
	{
		AbilityManagerComponent->OnOtherAbilityStart.AddUObject(this, &UAbilityComponent::OtherAbilityStart);
		AbilityManagerComponent->OnOtherAbilityEnd.AddUObject(this, &UAbilityComponent::OtherAbilityEnd);
		MakeUnavailableAbilityType = AbilityManagerComponent->GetMakeUnavailableAbilityTypes(AbilityType);
	}
}

void UAbilityComponent::UseAbility()
{
	if(CanUseAbility())
	{
		AbilityStart();
	}
}

bool UAbilityComponent::CanUseAbility()
{
	if(nullptr == AbilityManagerComponent)
	{
		return false;
	}

	// Check AbilityState
	// Check Manager CanUseAbility
	
	bool bCanUseAbility = true;
	
	bCanUseAbility &= (AbilityState == EAbilityState::EAS_Available);
	bCanUseAbility &= (AbilityManagerComponent->CanUseAbility(AbilityType));
	
	return bCanUseAbility;
}

void UAbilityComponent::AbilityStart()
{
	// Call AbilityManagerComponent this Ability Start
	// Change Ability State
	// Start Settings ( Capsule, Collision, etc.) in override
	// Play AnimMontage ( if montage exist ) in override

	AbilityManagerComponent->AbilityStart(AbilityType);
	SetAbilityState(EAbilityState::EAS_Active);
}

void UAbilityComponent::AbilityEnd()
{
	// Call AbilityManagerComponent this Ability End;
	// End Setting ( Capsule, Collision, etc.) in override

	AbilityManagerComponent->AbilityEnd(AbilityType);
}

void UAbilityComponent::CooldownStart()
{
	// SetAbilityState Cooldown
	// SetTimer
	
	if(AbilityState != EAbilityState::EAS_Unavailable)
	{
		SetAbilityState(EAbilityState::EAS_Cooldown);
	}
	
	if(OnAbilityCooldownTimeChanged.IsBound())
	{
		CurrentCooldownTime = CooldownTime;
		GetOwner()->GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &UAbilityComponent::CooldownTick, 0.1f, true, 0.1f);
	}
	else
	{
		GetOwner()->GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &UAbilityComponent::CooldownEnd, CooldownTime, false);
	}
}

void UAbilityComponent::CooldownEnd()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
	
	if(AbilityState != EAbilityState::EAS_Unavailable)
	{
		SetAbilityState(EAbilityState::EAS_Available);
	}
}

void UAbilityComponent::CooldownTick()
{
	CurrentCooldownTime -= 0.1f;
	OnAbilityCooldownTimeChanged.Broadcast(CurrentCooldownTime);

	if(FMath::IsNearlyZero(CurrentCooldownTime, 0.01f) || CurrentCooldownTime <= 0.f)
	{
		CooldownEnd();
	}
}

void UAbilityComponent::SetAbilityState(EAbilityState InAbilityState)
{
	AbilityState = InAbilityState;
	if(OnAbilityStateChanged.IsBound())
	{
		OnAbilityStateChanged.Broadcast(AbilityState);
	}
}

void UAbilityComponent::OtherAbilityStart(EAbilityType OtherAbilityType)
{
	if(MakeUnavailableAbilityType & static_cast<uint8>(OtherAbilityType))
	{
		SetAbilityState(EAbilityState::EAS_Unavailable);
	}
}

void UAbilityComponent::OtherAbilityEnd(EAbilityType OtherAbilityType)
{
	if(MakeUnavailableAbilityType & static_cast<uint8>(OtherAbilityType))
	{
		if(GetOwner()->GetWorldTimerManager().IsTimerActive(CooldownTimerHandle))
		{
			SetAbilityState(EAbilityState::EAS_Cooldown);
		}
		else
		{
			SetAbilityState(EAbilityState::EAS_Available);
		}
	}
}

void UAbilityComponent::PlayAbilityMontage()
{
	PlayableCharacter->GetMesh()->GetAnimInstance()->Montage_Play(AbilityMontage);
}

void UAbilityComponent::PlayAbilityMontage_JumpToSection(FName InSectionName)
{
	UAnimInstance* AnimInstance = PlayableCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AbilityMontage);
	AnimInstance->Montage_JumpToSection(InSectionName, AbilityMontage);
}

void UAbilityComponent::StopAbilityMontage(float InBlendOutTime)
{
	PlayableCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(InBlendOutTime, AbilityMontage);
}

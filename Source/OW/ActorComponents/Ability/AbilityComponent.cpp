// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"

#include "OW/Character/OWCharacterPlayable.h"


UAbilityComponent::UAbilityComponent() : CooldownTime(0.f), CurrentCooldownTime(0.f), AbilityType(EAbilityType::EAT_None),
AbilityState(EAbilityState::EAS_Available), CharacterPlayable(nullptr), AbilityMontage(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UAbilityComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if(AOWCharacterPlayable* OwnerCharacter = Cast<AOWCharacterPlayable>(GetOwner()))
	{
		CharacterPlayable = OwnerCharacter;
	}
	
	CharacterPlayable->AddAbilityStateChangedDelegate(AbilityType, FOnAbilityStateChangedDelegateWrapper(OnAbilityStateChanged));
	CharacterPlayable->AddAbilityCooldownTimeChangedDelegate(AbilityType, FOnAbilityCooldownTimeChangedDelegateWrapper(OnAbilityCooldownTimeChanged));

	CharacterPlayable->OnAbilityStart.AddUObject(this, &UAbilityComponent::OtherAbilityStart);
	CharacterPlayable->OnAbilityEnd.AddUObject(this, &UAbilityComponent::OtherAbilityEnd);
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
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
	bool bCanUseAbility = true;
	EAbilityType CurrentAbilityType = CharacterPlayable->GetCurrentAbilityType();
	bool bCanCancel = (CancellableAbilityTypes & static_cast<uint8>(CurrentAbilityType)) || (CurrentAbilityType == EAbilityType::EAT_None);
	
	bCanUseAbility &= (AbilityState == EAbilityState::EAS_Available);
	bCanUseAbility &= !(CharacterPlayable->GetQuickMeleeActive());
	bCanUseAbility &= bCanCancel;
	
	return bCanUseAbility;
}

void UAbilityComponent::AbilityStart()
{
	CharacterPlayable->AbilityStart(AbilityType);
	SetAbilityState(EAbilityState::EAS_Active);
}

void UAbilityComponent::AbilityEnd()
{
	CharacterPlayable->AbilityEnd(AbilityType);
}

void UAbilityComponent::CooldownStart()
{
	if(AbilityState != EAbilityState::EAS_Unavailable)
	{
		SetAbilityState(EAbilityState::EAS_Cooldown);
	}
	
	if(OnAbilityCooldownTimeChanged.IsBound())
	{
		CurrentCooldownTime = CooldownTime;
		OnAbilityCooldownTimeChanged.Broadcast(CooldownTime, CurrentCooldownTime);
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
	OnAbilityCooldownTimeChanged.Broadcast(CooldownTime, CurrentCooldownTime);

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
	if(MakeUnavailableAbilityTypes & static_cast<uint8>(OtherAbilityType))
	{
		SetAbilityState(EAbilityState::EAS_Unavailable);
	}
}

void UAbilityComponent::OtherAbilityEnd(EAbilityType OtherAbilityType)
{
	if(MakeUnavailableAbilityTypes & static_cast<uint8>(OtherAbilityType))
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

void UAbilityComponent::PlayAbilityMontage(UAnimMontage* InAbilityMontage)
{
	if(nullptr == InAbilityMontage) return;
	
	CharacterPlayable->GetMesh()->GetAnimInstance()->Montage_Play(InAbilityMontage);
}

void UAbilityComponent::PlayAbilityMontage_JumpToSection(UAnimMontage* InAbilityMontage, FName InSectionName)
{
	if(nullptr == InAbilityMontage) return;
	UE_LOG(LogTemp, Warning, TEXT("Deflect!"));
	UAnimInstance* AnimInstance = CharacterPlayable->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(InAbilityMontage);
	AnimInstance->Montage_JumpToSection(InSectionName, InAbilityMontage);
}

void UAbilityComponent::StopAbilityMontage(float InBlendOutTime)
{
	CharacterPlayable->GetMesh()->GetAnimInstance()->Montage_Stop(InBlendOutTime, AbilityMontage);
}

void UAbilityComponent::InitWidget()
{
	SetAbilityState(AbilityState);

	if(OnAbilityCooldownTimeChanged.IsBound())
	{
		OnAbilityCooldownTimeChanged.Broadcast(CooldownTime, CooldownTime);
	}
}

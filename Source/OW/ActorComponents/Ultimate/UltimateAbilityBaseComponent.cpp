// Fill out your copyright notice in the Description page of Project Settings.


#include "UltimateAbilityBaseComponent.h"

#include "OW/Character/OWCharacterPlayable.h"


UUltimateAbilityBaseComponent::UUltimateAbilityBaseComponent() : AbilityType(EAbilityType::EAT_AbilityThree)
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UUltimateAbilityBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if(AOWCharacterPlayable* Playable = Cast<AOWCharacterPlayable>(GetOwner()))
	{
		CharacterPlayable = Playable;
	}
}

void UUltimateAbilityBaseComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UUltimateAbilityBaseComponent::UseUltimate()
{
	if(CheckAvailable())
	{
		StartUltimate();
	}
}

bool UUltimateAbilityBaseComponent::CheckAvailable()
{
	bool bAvailable = true;
	EAbilityType CurrentAbilityType = CharacterPlayable->GetCurrentAbilityType();
	bool bCanCancel = (CancellableAbilityTypes & static_cast<uint8>(CurrentAbilityType)) || (CurrentAbilityType == EAbilityType::EAT_None);

	// 캐릭터 상태 검사

	bAvailable &= bCanCancel;
	bAvailable &= CharacterPlayable->IsUltimateGaugeFull();

	return bAvailable;
}

void UUltimateAbilityBaseComponent::StartUltimate()
{
	CharacterPlayable->UltimateUsed();
	CharacterPlayable->AbilityStart(EAbilityType::EAT_AbilityThree);
	CharacterPlayable->SetUltimateActive(true);
}

void UUltimateAbilityBaseComponent::EndUltimate()
{
	CharacterPlayable->AbilityEnd(EAbilityType::EAT_AbilityThree);
}

void UUltimateAbilityBaseComponent::PlayUltimateMontage()
{
	if(UltimateMontage)
	{
		CharacterPlayable->GetMesh()->GetAnimInstance()->Montage_Play(UltimateMontage);
	}
}
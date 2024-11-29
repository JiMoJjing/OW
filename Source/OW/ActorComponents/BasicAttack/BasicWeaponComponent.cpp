// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicWeaponComponent.h"

#include "OW/Character/OWCharacterPlayable.h"


UBasicWeaponComponent::UBasicWeaponComponent() : DelayTime(1.f), bDelayActive(false), MaxAmmo(30), ReloadDelayTime(0.912f)
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	CurrentAmmo = MaxAmmo;
}

void UBasicWeaponComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if(IOWPlayerTraceInterface* TraceInterface = Cast<IOWPlayerTraceInterface>(GetOwner()))
	{
		PlayerTraceInterface = TScriptInterface<IOWPlayerTraceInterface>(GetOwner());
	}

	if(IOWTriggerAnimNotifyInterface* AnimNotifyInterface = Cast<IOWTriggerAnimNotifyInterface>(GetOwner()))
	{
		TriggerAnimNotifyInterface = TScriptInterface<IOWTriggerAnimNotifyInterface>(GetOwner());
	}

	if(AOWCharacterPlayable* Character = Cast<AOWCharacterPlayable>(GetOwner()))
	{
		CharacterPlayable = Character;
	}

	CharacterPlayable->GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &UBasicWeaponComponent::OnMontageBlendingOut);
}

void UBasicWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UBasicWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBasicWeaponComponent::UseBasicWeapon(float InPlayRate)
{
	if(CheckAmmo() == false)
	{
		return;
	}
	if(CheckAvailable())
	{
		CharacterPlayable->OnAnimNotify.AddUObject(this, &UBasicWeaponComponent::Fire);
		PlayBasicWeaponMontage(InPlayRate);
		DelayStart(DelayTime);
	}
}

bool UBasicWeaponComponent::CheckAvailable()
{
	bool bAvailable = true;
	bAvailable &= (CharacterPlayable->GetCurrentAbilityType() == EAbilityType::EAT_None);
	bAvailable &= !(CharacterPlayable->GetQuickMeleeActive());
	bAvailable &= !bDelayActive;

	return bAvailable;
}

void UBasicWeaponComponent::Fire()
{
	// Make Fire
}

void UBasicWeaponComponent::PlayBasicWeaponMontage(float InPlayRate)
{
	if(BasicWeaponMontage)
	{
		CharacterPlayable->GetMesh()->GetAnimInstance()->Montage_Play(BasicWeaponMontage, InPlayRate);
	}
}

void UBasicWeaponComponent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage == BasicWeaponMontage || Montage == ReloadMontage)
	{
		CharacterPlayable->OnAnimNotify.RemoveAll(this);
	}
}

void UBasicWeaponComponent::DelayStart(float InDelayTime)
{
	bDelayActive = true;
	CharacterPlayable->GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &UBasicWeaponComponent::DelayEnd, InDelayTime, false);
}

void UBasicWeaponComponent::DelayEnd()
{
	bDelayActive = false;
}

bool UBasicWeaponComponent::CheckAmmo()
{
	return CurrentAmmo > 0;
}

void UBasicWeaponComponent::UseAmmo(uint8 InAmount)
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo - InAmount, 0, MaxAmmo);
	AmmoChanged();
}

void UBasicWeaponComponent::AmmoChanged()
{
	if(OnAmmoChanged.IsBound())
	{
		OnAmmoChanged.Broadcast(MaxAmmo, CurrentAmmo);
	}
}

void UBasicWeaponComponent::InitWidget()
{
	AmmoChanged();
}

void UBasicWeaponComponent::PlayReloadMontage(float InPlayRate)
{
	if(ReloadMontage)
	{
		CharacterPlayable->GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontage, InPlayRate);
	}
}

void UBasicWeaponComponent::Reload()
{
	CurrentAmmo = MaxAmmo;
	AmmoChanged();
}

void UBasicWeaponComponent::UseReload(float InPlayRate)
{
	if(CurrentAmmo == MaxAmmo)
	{
		return;
	}
	
	if(CheckAvailable())
	{
		CharacterPlayable->OnAnimNotify.AddUObject(this, &UBasicWeaponComponent::Reload);
		PlayReloadMontage(InPlayRate);
		DelayStart(ReloadDelayTime);
	}
}
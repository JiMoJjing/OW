// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHUD.h"

#include "AbilityWidget.h"
#include "AmmoWidget.h"
#include "PlayerHPBarWidget.h"
#include "OW/ActorComponents/BasicAttack/BasicWeaponComponent.h"
#include "OW/Character/OWCharacterPlayable.h"
#include "OW/Status/HPComponent.h"

UOWHUD::UOWHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), HitMarkerPositionOffset(FVector2D(38.f, 38.f))
{
	static ConstructorHelpers::FClassFinder<UUserWidget> HeadShotHitMarkerClassRef(TEXT("/Game/OW/Widget/PlayerHUD/WBP_HeadShotHitMarker.WBP_HeadShotHitMarker_C"));
	if(HeadShotHitMarkerClassRef.Class)
	{
		HeadShotHitMarker = HeadShotHitMarkerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> NormalHitMarkerClassRef(TEXT("/Game/OW/Widget/PlayerHUD/WBP_NormalHitMarker.WBP_NormalHitMarker_C"));
	if(NormalHitMarkerClassRef.Class)
	{
		NormalHitMarker = NormalHitMarkerClassRef.Class;
	}
}

void UOWHUD::NativeConstruct()
{
	Super::NativeConstruct();

	AbilityWidgets.Emplace(AbilityOneWidget->GetAbilityType(), AbilityOneWidget);
	AbilityWidgets.Emplace(AbilityTwoWidget->GetAbilityType(), AbilityTwoWidget);

	BindCharacterDelegate();
}

void UOWHUD::BindCharacterDelegate()
{
	AOWCharacterPlayable* CharacterPlayable = Cast<AOWCharacterPlayable>(GetOwningPlayerPawn());
	
	if(nullptr == CharacterPlayable) return;
	
	// HPComponent Bind
	if(UHPComponent* HPComponent = CharacterPlayable->GetHPComponent())
	{
		HPComponent->OnHPChanged.AddUObject(this, &UOWHUD::PlayerHPBarUpdate);
	}
	
	// AbilityComponent Bind

	if(CharacterPlayable->GetAbilityStateChangedDelegateWrapper(AbilityOneWidget->GetAbilityType()).IsValid())
	{
		CharacterPlayable->GetAbilityStateChangedDelegateWrapper(AbilityOneWidget->GetAbilityType()).OnAbilityStateChanged->AddUObject(AbilityOneWidget, &UAbilityWidget::AbilityStateChanged);
	}
	if(CharacterPlayable->GetAbilityCooldownTimeChangedDelegateWrapper(AbilityOneWidget->GetAbilityType()).IsValid())
	{
		CharacterPlayable->GetAbilityCooldownTimeChangedDelegateWrapper(AbilityOneWidget->GetAbilityType()).OnAbilityCooldownTimeChanged->AddUObject(AbilityOneWidget, &UAbilityWidget::CooldownTimeChanged);
	}

	if(CharacterPlayable->GetAbilityStateChangedDelegateWrapper(AbilityTwoWidget->GetAbilityType()).IsValid())
	{
		CharacterPlayable->GetAbilityStateChangedDelegateWrapper(AbilityTwoWidget->GetAbilityType()).OnAbilityStateChanged->AddUObject(AbilityTwoWidget, &UAbilityWidget::AbilityStateChanged);
	}
	if(CharacterPlayable->GetAbilityCooldownTimeChangedDelegateWrapper(AbilityTwoWidget->GetAbilityType()).IsValid())
	{
		CharacterPlayable->GetAbilityCooldownTimeChangedDelegateWrapper(AbilityTwoWidget->GetAbilityType()).OnAbilityCooldownTimeChanged->AddUObject(AbilityTwoWidget, &UAbilityWidget::CooldownTimeChanged);
	}
	

	// Ammo Bind
	if(UBasicWeaponComponent* BasicWeaponComponent = CharacterPlayable->GetBasicWeaponComponent())
	{
		BasicWeaponComponent->OnAmmoChanged.AddUObject(this, &UOWHUD::AmmoWidgetUpdate);
	}
	
	CharacterPlayable->InitWidget();
}

void UOWHUD::PlayerHPBarUpdate(float InMaxHP, float InCurrentHP)
{
	PlayerHPBar->OnHPChanged(InMaxHP, InCurrentHP);
}

void UOWHUD::CreateHitMarker(bool bIsHeadShot)
{
	FVector2D ViewportCenterPosition = GetPaintSpaceGeometry().GetLocalSize() / 2.f;
	
	if(bIsHeadShot)
	{
		if(HeadShotHitMarker)
		{
			UUserWidget* HitMarker = CreateWidget(GetOwningPlayer(), HeadShotHitMarker);
			HitMarker->SetPositionInViewport(ViewportCenterPosition - HitMarkerPositionOffset, false);
			HitMarker->AddToViewport();
		}
	}
	else
	{
		if(NormalHitMarker)
		{
			UUserWidget* HitMarker = CreateWidget(GetOwningPlayer(), NormalHitMarker);
			HitMarker->SetPositionInViewport(ViewportCenterPosition - HitMarkerPositionOffset, false);
			HitMarker->AddToViewport();
		}
	}
}

void UOWHUD::AmmoWidgetUpdate(uint8 InMaxAmmo, uint8 InCurrentAmmo)
{
	AmmoWidget->AmmoChanged(InMaxAmmo, InCurrentAmmo);
}

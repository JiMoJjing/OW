// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHUD.h"

#include "AbilityWidget.h"
#include "PlayerHPBarWidget.h"
#include "OW/Ability/AbilityComponent.h"
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

	AbilityWidgets.Add(AbilityOneWidget->GetAbilityType(), AbilityOneWidget);
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
	CharacterPlayable->GetAbilityStateChangedDelegateWrapper(AbilityOneWidget->GetAbilityType()).OnAbilityStateChanged->AddUObject(AbilityOneWidget, &UAbilityWidget::AbilityStateChanged);
	CharacterPlayable->GetAbilityCooldownTimeChangedDelegateWrapper(AbilityOneWidget->GetAbilityType()).OnAbilityCooldownTimeChanged->AddUObject(AbilityOneWidget, &UAbilityWidget::CooldownTimeChanged);
	
	CharacterPlayable->InitializeWidget();
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
// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "OW/ActorComponents/Ability/AbilityState.h"

void UAbilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(AbilityTexture)
	{
		AbilityImage->SetBrushFromTexture(AbilityTexture);
	}
}

void UAbilityWidget::AbilityStateChanged(EAbilityState InAbilityState)
{
	switch (InAbilityState)
	{
	case EAbilityState::EAS_Available:
		SetAvailableState();
		break;
	case EAbilityState::EAS_Active:
		SetActiveState();
		break;
	case EAbilityState::EAS_Cooldown:
		SetCooldownState();
		break;
	case EAbilityState::EAS_Unavailable:
		SetUnavailableState();
		break;
	}
}

void UAbilityWidget::CooldownTimeChanged(float CooldownTime, float CurrentCooldownTime)
{
	CooldownProgressBar->SetPercent(1.f - CurrentCooldownTime / CooldownTime);
	CooldownTextBlock->SetText(FText::FromString(FString::FromInt(FMath::CeilToInt(CurrentCooldownTime))));
}

void UAbilityWidget::SetAvailableState()
{
	CooldownTextBlock->SetVisibility(ESlateVisibility::Hidden);
	CooldownProgressBar->SetVisibility(ESlateVisibility::Hidden);
	BackgroundImage->SetBrushTintColor(BackgroundImageColor_Available);
}

void UAbilityWidget::SetActiveState()
{
	CooldownTextBlock->SetVisibility(ESlateVisibility::Hidden);
	CooldownProgressBar->SetVisibility(ESlateVisibility::Hidden);
	BackgroundImage->SetBrushTintColor(BackgroundImageColor_Active);
}

void UAbilityWidget::SetCooldownState()
{
	CooldownTextBlock->SetVisibility(ESlateVisibility::Visible);
	CooldownProgressBar->SetVisibility(ESlateVisibility::Visible);
	BackgroundImage->SetBrushTintColor(BackgroundImageColor_Cooldown);
}

void UAbilityWidget::SetUnavailableState()
{
	CooldownProgressBar->SetVisibility(ESlateVisibility::Hidden);
	BackgroundImage->SetBrushTintColor(BackgroundImageColor_Unavailable);
}

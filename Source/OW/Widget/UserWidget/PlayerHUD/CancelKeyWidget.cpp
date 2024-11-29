// Fill out your copyright notice in the Description page of Project Settings.


#include "CancelKeyWidget.h"

#include "Components/TextBlock.h"

void UCancelKeyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);
}

void UCancelKeyWidget::AbilityStateChanged(EAbilityState InAbilityState)
{
	if(InAbilityState == EAbilityState::EAS_Active)
	{
		SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(CancelWidgetAnimation);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCancelKeyWidget::SetKeyText(FName InKeyName) const
{
	KeyNameTextBlock->SetText(FText::FromName(InKeyName));
}

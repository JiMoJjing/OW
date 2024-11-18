// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingHPBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UFloatingHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFloatingHPBarWidget::OnHPChanged(float InMaxHP, float InCurrentHP)
{
	HPProgressBar->SetPercent(InCurrentHP / InMaxHP);
}

void UFloatingHPBarWidget::SetNameText(const FText& InNameText)
{
	NameTextBlock->SetText(InNameText);
}
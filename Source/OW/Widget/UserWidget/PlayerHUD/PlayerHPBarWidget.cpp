// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHPBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerHPBarWidget::OnHPChanged(float InMaxHP, float InCurrentHP)
{
	float Percentage = InCurrentHP / InMaxHP;
	
	uint8 MaxHP = static_cast<uint8>(InMaxHP);
	uint8 CurrentHP = static_cast<uint8>(InCurrentHP);

	HPProgressBar->SetPercent(Percentage);
	
	FText HPText = FText::FromString(FString::FromInt(CurrentHP) + " / " + FString::FromInt(MaxHP));
	
	HPTextBlock->SetText(HPText);
}

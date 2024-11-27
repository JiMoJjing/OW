// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(AmmoImageTexture)
	{
		AmmoImage->SetBrushFromTexture(AmmoImageTexture);
	}
}

void UAmmoWidget::AmmoChanged(uint8 InMaxAmmo, uint8 InCurrentAmmo)
{
	FText AmmoText = FText::FromString(FString::FromInt(InCurrentAmmo) + " / " + FString::FromInt(InMaxAmmo));
	AmmoTextBlock->SetText(AmmoText);
}

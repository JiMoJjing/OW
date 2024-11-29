// Fill out your copyright notice in the Description page of Project Settings.


#include "UltimateGaugeWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UUltimateGaugeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UMaterialInstance* MaterialInstace = Cast<UMaterialInstance>(UltimateGaugeProgressBar->GetBrush().GetResourceObject());
	MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(MaterialInstace, nullptr);
	UltimateGaugeProgressBar->SetBrushResourceObject(MaterialInstanceDynamic);

	if(UltimateImageTexture)
	{
		UltimateImage->SetBrushFromTexture(UltimateImageTexture);
	}
	
	UltimateImage->SetVisibility(ESlateVisibility::Collapsed);
}

void UUltimateGaugeWidget::OnUltimateGaugeChanged(float InMaxUltimateGauge, float InCurrentUltimateGauge)
{
	float PercentageFloat = InCurrentUltimateGauge / InMaxUltimateGauge;
	
	if(PercentageFloat >= 1.f)
	{
		UltimateImage->SetVisibility(ESlateVisibility::Visible);
		UltimateGaugeTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		UltimateImage->SetVisibility(ESlateVisibility::Collapsed);
		UltimateGaugeTextBlock->SetVisibility(ESlateVisibility::Visible);
	}

	uint8 PercentageInt = static_cast<uint8>(PercentageFloat * 100.f);
	FText Text = FText::FromString(FString::FromInt(PercentageInt));
	UltimateGaugeTextBlock->SetText(Text);
	MaterialInstanceDynamic->SetScalarParameterValue(TEXT("Percent"), PercentageFloat);
}

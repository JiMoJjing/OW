// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UltimateGaugeWidget.generated.h"

class UImage;
class UTextBlock;
class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS()
class OW_API UUltimateGaugeWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnUltimateGaugeChanged(float InMaxUltimateGauge, float InCurrentUltimateGauge);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> UltimateGaugeTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> UltimateGaugeProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> UltimateImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTexture2D> UltimateImageTexture;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstanceDynamic;
};

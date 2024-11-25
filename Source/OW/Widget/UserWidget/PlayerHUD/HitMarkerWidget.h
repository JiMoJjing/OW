// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitMarkerWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class OW_API UHitMarkerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HitMarkerAnimationEnded();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> HitMarkerImage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HitMarkerWidgetAnimation;
};

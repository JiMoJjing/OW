// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class OW_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> CrosshairImage;
};

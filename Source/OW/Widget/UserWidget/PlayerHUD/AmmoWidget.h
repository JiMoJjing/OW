// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class OW_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void AmmoChanged(uint8 InMaxAmmo, uint8 InCurrentAmmo);

protected:
	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> BackgroundImage;

	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> AmmoImage;

	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> AmmoTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTexture2D> AmmoImageTexture;
};

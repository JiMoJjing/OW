// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPBarWidget.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class OW_API UPlayerHPBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void OnHPChanged(float InMaxHP, float InCurrentHP);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> CharacterImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UProgressBar> HPProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> HPTextBlock;
};

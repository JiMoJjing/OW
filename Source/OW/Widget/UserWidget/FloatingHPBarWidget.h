// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingHPBarWidget.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class OW_API UFloatingHPBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void OnHPChanged(float InMaxHP, float InCurrentHP);
	void SetNameText(const FText& InNameText);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FloatingHPBarWidget, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UProgressBar> HPProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FloatingHPBarWidget, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> NameTextBlock;
};

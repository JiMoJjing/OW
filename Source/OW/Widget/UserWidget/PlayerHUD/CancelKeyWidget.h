// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OW/ActorComponents/Ability/AbilityState.h"
#include "OW/ActorComponents/Ability/AbilityType.h"
#include "CancelKeyWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class OW_API UCancelKeyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void AbilityStateChanged(EAbilityState InAbilityState);

	UFUNCTION()
	void SetKeyText(FName InKeyName) const;

	EAbilityType GetAbilityType() const { return AbilityType; }
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EAbilityType AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> CancelTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> KeyImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> KeyNameTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (AllowPrivateAccess = "true", BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CancelWidgetAnimation;
};

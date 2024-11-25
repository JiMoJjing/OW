// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OW/Ability/AbilityState.h"
#include "OW/Ability/AbilityType.h"
#include "AbilityWidget.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class OW_API UAbilityWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

public:
	void AbilityStateChanged(EAbilityState InAbilityState);
	void CooldownTimeChanged(float CooldownTime, float CurrentCooldownTime);
	
	EAbilityType GetAbilityType() const { return AbilityType; }
	
protected:
	void SetAvailableState();
	void SetActiveState();
	void SetCooldownState();
	void SetUnavailableState();

protected:
	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> BackgroundImage;

	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> AbilityImage;

	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UProgressBar> CooldownProgressBar;
	
	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> CooldownTextBlock;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EAbilityType AbilityType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTexture2D> AbilityTexture;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FSlateColor BackgroundImageColor_Available;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FSlateColor BackgroundImageColor_Active;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FSlateColor BackgroundImageColor_Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FSlateColor BackgroundImageColor_Unavailable;
};

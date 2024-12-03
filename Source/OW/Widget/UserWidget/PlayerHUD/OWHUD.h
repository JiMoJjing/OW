// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OW/ActorComponents/Ability/AbilityType.h"
#include "OWHUD.generated.h"


class UUltimateGaugeWidget;
class UCancelKeyWidget;
class UAmmoWidget;
class UAbilityWidget;
class UPlayerHPBarWidget;

UCLASS()
class OW_API UOWHUD : public UUserWidget
{
	GENERATED_BODY()

	UOWHUD(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	
	void PlayerHPBarUpdate(float InMaxHP, float InCurrentHP);
	void AmmoWidgetUpdate(uint8 InMaxAmmo, uint8 InCurrentAmmo);
	
public:
	void BindCharacterDelegate();
	void CreateHitMarker(bool bIsHeadShot);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OWHUD, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UPlayerHPBarWidget> PlayerHPBar; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OWHUD, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UUserWidget> Crosshair;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HeadShotHitMarker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> NormalHitMarker;

	FVector2D HitMarkerPositionOffset;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UAbilityWidget> AbilityOneWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UAbilityWidget> AbilityTwoWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UUltimateGaugeWidget> UltimateGaugeWidget;

	TMap<EAbilityType, UAbilityWidget*> AbilityWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UAmmoWidget> AmmoWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UCancelKeyWidget> CancelKeyWidget;
};

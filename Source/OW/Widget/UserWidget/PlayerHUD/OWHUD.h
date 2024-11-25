// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OW/Ability/AbilityType.h"
#include "OWHUD.generated.h"


class UAbilityWidget;
class UPlayerHPBarWidget;

UCLASS()
class OW_API UOWHUD : public UUserWidget
{
	GENERATED_BODY()

	UOWHUD(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;


// Bind Character Delegate
public:
	void BindCharacterDelegate();


// PlayerHPBar
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UPlayerHPBarWidget> PlayerHPBar; 

public:
	void PlayerHPBarUpdate(float InMaxHP, float InCurrentHP);


// Crosshair
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UUserWidget> Crosshair;
	
	
// HitMarker	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HeadShotHitMarker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> NormalHitMarker;

	UPROPERTY()
	FVector2D HitMarkerPositionOffset;

public:
	void CreateHitMarker(bool bIsHeadShot);


// Ability Widget
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UAbilityWidget> AbilityOneWidget;

	UPROPERTY()
	TMap<EAbilityType, TObjectPtr<UAbilityWidget>> AbilityWidgets;
};

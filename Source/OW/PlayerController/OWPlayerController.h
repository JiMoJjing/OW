// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "OW/Interface/OWApplyDamageInterface.h"
#include "OWPlayerController.generated.h"

/**
 * 
 */

class UOWHUD;
class IOWCharacterInputInterface;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(Log_Input_Controller, Log, All);

UCLASS()
class OW_API AOWPlayerController : public APlayerController, public IOWApplyDamageInterface
{
	GENERATED_BODY()

public:
	AOWPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	virtual void SetupInputComponent() override;

// Input Section
protected:
	void Move(const FInputActionValue& Value);
	void MoveReleased();
	void Look(const FInputActionValue& Value);
	void Jump();

	void OnJumpPressed();
	void OnJumpReleased();

	void PrimaryFire();
	void SecondaryFire();
	void AbilityOne();
	void AbilityTwo();
	void AbilityThree();
	void Reload();
	void QuickMelee();

public:
	virtual void ApplyDamageSuccess(float Damage, bool bIsHeadShot) override;
	virtual void KillSuccess() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PrimaryFireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SecondaryFireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AbilityOneAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AbilityTwoAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AbilityThreeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> QuickMeleeAction;

	UPROPERTY()
	TScriptInterface<IOWCharacterInputInterface> CharacterInputInterface;

	UPROPERTY()
	TScriptInterface<IOWApplyDamageInterface> CharacterApplyDamageInterface;

	UPROPERTY()
	TScriptInterface<IOWApplyDamageInterface> HUDApplyDamageInterface;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD, meta = (AllowPrivateAccess = "ture"))
	TSubclassOf<UUserWidget> OWHUDClass;

	UPROPERTY()
	TObjectPtr<UOWHUD> OWHUD;
};

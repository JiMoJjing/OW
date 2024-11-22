// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/Character/OWCharacterBase.h"
#include "OWStandardBot.generated.h"


class UHPBarWidgetComponent;

UCLASS()
class OW_API AOWStandardBot : public AOWCharacterBase
{
	GENERATED_BODY()

public:
	AOWStandardBot();

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

	virtual void CharacterDeath() override;
	virtual void CharacterRevive() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

// WidgetComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WidgetComponent, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHPBarWidgetComponent> HPBarWidgetComponent;

	void SetWidgetComponentVisibility(bool bNewVisibility);
	
// AnimMontage
	UPROPERTY()
	TObjectPtr<UAnimMontage> ReviveAnimMontage;

// Collision Section	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> ArmLCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> ArmRCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> LegCollision;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/Character/OWCharacterBase.h"
#include "OW/Interface/OWTriggerAnimNotifyInterface.h"
#include "OWStandardBot.generated.h"


class UProjectileWeaponComponent;
class AOWProjectileBase;
class UHPBarWidgetComponent;

UCLASS()
class OW_API AOWStandardBot : public AOWCharacterBase, public IOWTriggerAnimNotifyInterface
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
	void SetWidgetComponentVisibility(bool bNewVisibility);
	void PlayFireMontage();
	void StandardBotFire();

public:
	virtual void TriggerAnimNotify() override;
	virtual void TriggerAnimNotifyBegin() override;
	virtual void TriggerAnimNotifyEnd() override;
	virtual void TriggerAnimNotifyState(float Delta) override;;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> ArmLCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> ArmRCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> LegCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_AnimMontage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ReviveAnimMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StandardBot_WidgetComponent, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHPBarWidgetComponent> HPBarWidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileWeaponComponent> ProjectileWeaponComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> LeftMuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RightMuzzle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_AnimMontage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> StandardBotFireMontage;

	uint8 bMuzzleChange : 1;
};

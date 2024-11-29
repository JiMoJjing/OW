// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/Character/OWCharacterBase.h"
#include "OW/Interface/OWTriggerAnimNotifyInterface.h"
#include "OWStandardBot.generated.h"


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

// WidgetComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StandardBot_WidgetComponent, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHPBarWidgetComponent> HPBarWidgetComponent;

	void SetWidgetComponentVisibility(bool bNewVisibility);
	
// AnimMontage
	UPROPERTY()
	TObjectPtr<UAnimMontage> ReviveAnimMontage;

// Collision Section	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> ArmLCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> ArmRCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> LegCollision;


// Attack Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> LeftMuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RightMuzzle;

	UPROPERTY()
	TObjectPtr<UAnimMontage> StandardBotFireMontage;
	
	UPROPERTY()
	TSubclassOf<AOWProjectileBase> StandardBotProjectileClass;

	UPROPERTY()
	TArray<TObjectPtr<AOWProjectileBase>> ProjectilePool;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Projectile, meta = (AllowPrivateAccess = "true"))
	uint8 PoolSize;

	UPROPERTY()
	uint8 PoolIndex;
	
	void FillProjectilePool();

	TObjectPtr<AOWProjectileBase> GetProjectileFromPool();

	void PlayFireMontage();
	
	void StandardBotFire();

	FTimerHandle FireModeTimerHandle;
	
	void StartStandardBotFireMode();

	void StopStandardBotFireMode();


// IOWTriggerAnimNotifyInterface
public:
	virtual void TriggerAnimNotify() override;
	virtual void TriggerAnimNotifyBegin() override;
	virtual void TriggerAnimNotifyEnd() override;
	virtual void TriggerAnimNotifyState(float Delta) override;;
};

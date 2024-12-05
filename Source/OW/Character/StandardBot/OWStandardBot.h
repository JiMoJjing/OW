// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/Character/OWCharacterBase.h"
#include "OW/Interface/OWAICharacterInterface.h"
#include "OW/Interface/OWTriggerAnimNotifyInterface.h"
#include "OWStandardBot.generated.h"


class AOWAIController;
class UProjectileWeaponComponent;
class AOWProjectileBase;
class UHPBarWidgetComponent;

UCLASS()
class OW_API AOWStandardBot : public AOWCharacterBase, public IOWTriggerAnimNotifyInterface, public IOWAICharacterInterface
{
	GENERATED_BODY()

public:
	AOWStandardBot();
	virtual void PostInitializeComponents() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void CharacterDeath() override;
	virtual void CharacterRevive() override;
	void CharacterReviveRunAI();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void SetWidgetComponentVisibility(bool bNewVisibility);
	void PlayFireMontage();
	void StandardBotFire();
	void AttackFinished(UAnimMontage* Montage, bool bInterrupted);
	void SpawnProjectile();
	AOWProjectileBase* GetProjectileFromPool();

	
public:
	virtual void TriggerAnimNotify() override;
	virtual void TriggerAnimNotifyBegin() override;
	virtual void TriggerAnimNotifyEnd() override;
	virtual void TriggerAnimNotifyState(float Delta) override;

	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITrunSpeed() override;
	virtual FVector GetAIPatrolPos() override;
	virtual ECollisionChannel GetAITraceChannel() override;
	
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AIAttack() override;
	

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
	TObjectPtr<USceneComponent> LeftMuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RightMuzzle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_AnimMontage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> StandardBotFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Projectile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOWProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_Projectile, meta = (AllowPrivateAccess = "true"))
	uint8 PoolSize;

	TArray<TObjectPtr<AOWProjectileBase>> ProjectilePool;
	
	uint8 PoolIndex;
	
	uint8 bMuzzleChange : 1;

	UPROPERTY()
	TObjectPtr<AOWAIController> OWAIController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_AI, meta = (AllowPrivateAccess = "true"))
	float AIDetectRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_AI, meta = (AllowPrivateAccess = "true"))
	float AIAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_AI, meta = (AllowPrivateAccess = "true"))
	float AITurnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StandardBot_AI, meta = (AllowPrivateAccess = "true"))
	FVector AIPatrolPos;

	FAICharacterAttackFinished OnAttachFinished;
};

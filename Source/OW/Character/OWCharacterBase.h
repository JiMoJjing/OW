// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OWCharacterBase.generated.h"

class UHPComponent;


UCLASS()
class OW_API AOWCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AOWCharacterBase();

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UHPComponent* GetHPComponent() { return HPComponent; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character_HP, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHPComponent> HPComponent;
	
	virtual void CharacterDeath();
	virtual void CharacterRevive();

// Initial Variable	
	UPROPERTY()
	FVector MeshRelativeLocation;

	UPROPERTY()
	FRotator MeshRelativeRotation;


// Collision Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> BodyCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> HeadCollision;

	UPROPERTY()
	TArray<TObjectPtr<UCapsuleComponent>> CollisionArray;

	void SetCollisionEnabled(ECollisionEnabled::Type InType);
	void SetCollisionProfileName(FName InCollisionProfileName);
};

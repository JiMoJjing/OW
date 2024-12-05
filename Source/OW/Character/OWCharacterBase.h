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
	virtual void CharacterDeath();
	virtual void CharacterRevive();

	UPROPERTY()
	FVector MeshRelativeLocation;

	UPROPERTY()
	FRotator MeshRelativeRotation;

	void SetMeshCollisionEnabled(ECollisionEnabled::Type InType);
	void SetMeshCollisionProfileName(FName InCollisionProfileName);

public:
	void SetMaxWalkSpeedByMultiplier(float InMultiplier);
	void SetMaxWalkSpeedToDefault();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character_HP, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHPComponent> HPComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> BodyCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterCollision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> HeadCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji, meta = (AllowPrivateAccess = true))
	float DefaultMaxWalkSpeed;
	
	TArray<TObjectPtr<UShapeComponent>> CollisionArray;

	FVector GameStartLocation;
	FRotator GameStartRotation;
};

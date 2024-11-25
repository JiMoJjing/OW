// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/Ability/AbilityComponent.h"
#include "GenjiSwiftStrikeComponent.generated.h"


class UNiagaraSystem;
class USphereComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UGenjiSwiftStrikeComponent : public UAbilityComponent
{
	GENERATED_BODY()

public:
	UGenjiSwiftStrikeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Ability Function Override	
private:
	virtual void AbilityStart() override;
	virtual void AbilityEnd() override;

// SwiftStrike Function	Section
	void SwiftStrikeStartSetting();
	void SwiftStrikeEndSetting();

	void SetSwiftStrikeStartLocation();
	void SetSwiftStrikeEndLocation();

	void SwiftStrikeUptade(float DeltaTime);

	UFUNCTION()
	void OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

public:
	void SwiftStrikeCooldownReset(); 

private:
// SwiftStrike Variable Section	
	float SwiftStrikeDistance;
	float SwiftStrikeSpeed;

	FVector SwiftStrikeStartLocation;
	FVector SwiftStrikeEndLocation;

	FVector SwiftStrikeHitNormalProjection;
	float HitNormalProjectionInterpSpeed;

	uint8 bCheckDoubleJump : 1;

	uint8 bSwiftStrikeCooldownReset : 1;

	
// CapsuleSize
	FVector2D CapsuleSize2D;

// Damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_SwiftStrike, meta = (AllowPrivateAccess = "true"))
	float SwiftStrikeDamage;


// Collider
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability_SwiftStrike, meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent> SwiftStrikeCollider;

	UFUNCTION()
	void OnSwiftStrikeColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TArray<TObjectPtr<AActor>> OverlappedActors;

// Niagara
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability_SwiftStrike, meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraSystem> SwiftStrikeHitEffect;
};

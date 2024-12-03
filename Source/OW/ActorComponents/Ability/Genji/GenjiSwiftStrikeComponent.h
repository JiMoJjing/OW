// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/ActorComponents/Ability/AbilityComponent.h"
#include "GenjiSwiftStrikeComponent.generated.h"


class AOWGenji;
class UNiagaraSystem;
class USphereComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UGenjiSwiftStrikeComponent : public UAbilityComponent
{
	GENERATED_BODY()

public:
	UGenjiSwiftStrikeComponent();
	virtual void InitializeComponent() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	virtual void AbilityStart() override;
	virtual void AbilityEnd() override;

	void SwiftStrikeStartSetting();
	void SwiftStrikeEndSetting();

	void SetSwiftStrikeStartLocation();
	void SetSwiftStrikeEndLocation();

	void SwiftStrikeUptade(float DeltaTime);

	UFUNCTION()
	void OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnSwiftStrikeColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void SwiftStrikeCooldownReset(); 

private:
	UPROPERTY()
	TObjectPtr<AOWGenji> GenjiRef;

	float SwiftStrikeDistance;
	float SwiftStrikeSpeed;

	FVector SwiftStrikeStartLocation;
	FVector SwiftStrikeEndLocation;

	FVector SwiftStrikeHitNormalProjection;
	float HitNormalProjectionInterpSpeed;

	uint8 bCheckDoubleJump : 1;
	uint8 bSwiftStrikeCooldownReset : 1;

	FVector2D CapsuleSize2D;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_SwiftStrike, meta = (AllowPrivateAccess = "true"))
	float SwiftStrikeDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_SwiftStrike, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SwiftStrikeCollider;

	TArray<TObjectPtr<AActor>> OverlappedActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability_SwiftStrike, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> SwiftStrikeHitEffect;

	FName DragonbladeStartSectionName;
	FName DragonbladeEndSectionName;
};

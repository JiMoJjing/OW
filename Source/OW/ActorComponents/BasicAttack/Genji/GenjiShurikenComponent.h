// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OW/ActorComponents/BasicAttack/ProjectileWeaponComponent.h"
#include "GenjiShurikenComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OW_API UGenjiShurikenComponent : public UProjectileWeaponComponent
{
	GENERATED_BODY()

public:
	UGenjiShurikenComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UseAltFire(float InPlayRate);

	void OnDragonbladeStart();

private:
	void AltFire();
	void PlayAltFireMontage(float InPlayRate);
	virtual void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji_Shuriken, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AltFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji_Shuriken, meta = (AllowPrivateAccess = "true"))
	float AltFireDelayTime;
};

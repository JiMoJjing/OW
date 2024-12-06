#pragma once

#include "CoreMinimal.h"
#include "OW/Character/OWCharacterPlayable.h"
#include "OW/Interface/OWCharacterInputInterface.h"
#include "OWGenji.generated.h"

class UGenjiDragonbladeComponent;
class UGenjiDeflectComponent;
class UGenjiShurikenComponent;
class UProjectileWeaponComponent;
class UGenjiSwiftStrikeComponent;

UCLASS()
class OW_API AOWGenji : public AOWCharacterPlayable, public IOWCharacterInputInterface
{
	GENERATED_BODY()

public:
	AOWGenji();

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void PrimaryFire() override;
	virtual void SecondaryFire() override;
	virtual void AbilityOne() override;
	virtual void AbilityTwo() override;
	virtual void AbilityThree() override;
	virtual void QuickMelee() override;
	virtual void Reload() override;
	virtual void Move(const FInputActionValue& Value) override;
	virtual void MoveReleased() override;
	virtual void JumpPressed() override;
	virtual void JumpReleased() override;
	virtual void KillSuccess() override;
	virtual void InitWidget() override;
	void OnDragonbladeStart();
	void DragonMontageStart();

	TObjectPtr<UGenjiDragonbladeComponent> GetDragonbladeComponent() { return GenjiDragonbladeComponent; }

	bool GetIsClimbWall() const { return bIsClimbWall; }

private:
	void ClimbWallStart(FVector& HitNormal);
	void ClimbWallEnd();
	void StopClimbing();
	virtual void Landed(const FHitResult& Hit) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGenjiShurikenComponent> GenjiShurikenComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGenjiSwiftStrikeComponent> GenjiSwiftStrikeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGenjiDeflectComponent> GenjiDeflectComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGenjiDragonbladeComponent> GenjiDragonbladeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji, meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> DragonMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DragonMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ClimbWallMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ClimbMantleMontage;
	
	uint8 bIsClimbWall : 1;
	uint8 bCanDoubleJump : 1;
	uint8 bCanClimbWall : 1;
};

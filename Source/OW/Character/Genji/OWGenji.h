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
	virtual void KillSuccess() override;
	virtual void InitWidget() override;
	void SwiftStrikeReset();

	TObjectPtr<UGenjiDragonbladeComponent> GetDragonbladeComponent() { return GenjiDragonbladeComponent; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGenjiShurikenComponent> GenjiShurikenComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGenjiSwiftStrikeComponent> GenjiSwiftStrikeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGenjiDeflectComponent> GenjiDeflectComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Genji, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGenjiDragonbladeComponent> GenjiDragonbladeComponent;

};

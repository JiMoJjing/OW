#pragma once

#include "CoreMinimal.h"
#include "OW/Character/OWCharacterPlayable.h"
#include "OWGenji.generated.h"

class UGenjiShurikenComponent;

UCLASS()
class OW_API AOWGenji : public AOWCharacterPlayable
{
	GENERATED_BODY()

public:
	AOWGenji();

	virtual void PostInitializeComponents() override;

	virtual void SecondaryFire() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void KillSuccess() override;

private:
	UPROPERTY()
	TObjectPtr<UGenjiShurikenComponent> GenjiShurikenComponent;
};

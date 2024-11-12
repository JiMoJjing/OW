#pragma once

#include "CoreMinimal.h"
#include "OW/Character/OWCharacterPlayable.h"
#include "OWGenji.generated.h"

UCLASS()
class OW_API AOWGenji : public AOWCharacterPlayable
{
	GENERATED_BODY()

public:
	AOWGenji();

protected:
	virtual void BeginPlay() override;

public:
	virtual void KillSuccess() override;
};

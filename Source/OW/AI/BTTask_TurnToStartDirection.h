// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToStartDirection.generated.h"

/**
 * 
 */
UCLASS()
class OW_API UBTTask_TurnToStartDirection : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TurnToStartDirection();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

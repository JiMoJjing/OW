// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AIAttack.generated.h"

/**
 * 
 */
UCLASS()
class OW_API UBTTask_AIAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AIAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

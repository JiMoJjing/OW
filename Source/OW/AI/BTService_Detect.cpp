// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"

#include "AIController.h"
#include "OWAIBBKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "OW/Interface/OWAICharacterInterface.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 3.f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(nullptr == ControllingPawn)
	{
		return;
	}

	UWorld* World = ControllingPawn->GetWorld();
	if(nullptr == World)
	{
		return;
	}
	
	IOWAICharacterInterface* AIPawn = Cast<IOWAICharacterInterface>(ControllingPawn);
	if(nullptr == AIPawn)
	{
		return;
	}

	FVector OverlapLocation = ControllingPawn->GetActorLocation();
	float DetectRange = AIPawn->GetAIDetectRange();
	ECollisionChannel AITraceChannel = AIPawn->GetAITraceChannel();
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn);

	bool bResult = World->OverlapMultiByChannel(OverlapResults, OverlapLocation, FQuat::Identity, AITraceChannel, FCollisionShape::MakeSphere(DetectRange), CollisionQueryParams);
	if(bResult)
	{
		for(const auto& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if(Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				DrawDebugSphere(World, OverlapLocation, DetectRange, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Blue, Pawn->GetName());
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	DrawDebugSphere(World, OverlapLocation, DetectRange, 16, FColor::Red, false, 0.2f);
	GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Red, TEXT("None"));
}

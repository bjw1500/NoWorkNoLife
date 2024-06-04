// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearObjectCount.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Interface/CombatInterface.h"
#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"

void UBTService_FindNearObjectCount::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//URPGBlueprintLibrary::DebugLog(TEXT("Test"));


	APawn* OwingPawn = AIOwner->GetPawn();

	/*Owner가 플레이어면 몬스터를 찾고, 몬스터면 플레이어를 찾는다*/

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwingPawn, TargetTag, ActorsWithTag);

	float ClosesetDistance = TNumericLimits<float>::Max();
	float Range = SearchRange;

	AActor* ClosesetActor = nullptr;
	int32 Count = 0;
	for (AActor* Actor : ActorsWithTag)
	{
		if (IsValid(Actor) && IsValid(OwingPawn) && !ICombatInterface::Execute_IsDead(Actor))
		{
			const float Distance = OwingPawn->GetDistanceTo(Actor);

			if (Distance > Range)
				continue;
			Count++;
		}
	}

	UBTFunctionLibrary::SetBlackboardValueAsInt(this, NearObjectCount, Count);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearItem.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Actors/Item/ItemBase.h"
#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"
#include "Characters/EnemyCharacter.h"

void UBTService_FindNearItem::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//URPGBlueprintLibrary::DebugLog(TEXT("Test"));


	APawn* OwingPawn = AIOwner->GetPawn();

	/*Owner가 플레이어면 몬스터를 찾고, 몬스터면 플레이어를 찾는다*/

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwingPawn, TargetTag, ActorsWithTag);

	float ClosesetDistance = TNumericLimits<float>::Max();
	float Range = UBTFunctionLibrary::GetBlackboardValueAsFloat(this, SearchRange);

	AActor* ClosesetActor = nullptr;
	for (AActor* Actor : ActorsWithTag)
	{
		if (IsValid(Actor) && IsValid(OwingPawn))
		{
			AItemBase* Item = Cast<AItemBase>(Actor);
			if (Item && Item->ItemOwner)
			{
				if(AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Item->ItemOwner))
					continue;
			}

			const float Distance = OwingPawn->GetDistanceTo(Actor);

			if(Distance > Range)
				continue;

			if (Distance < ClosesetDistance)
			{
				ClosesetDistance = Distance;
				ClosesetActor = Actor;
			}
		}
	}

	UBTFunctionLibrary::SetBlackboardValueAsObject(this, DiscoverItem, ClosesetActor);
}

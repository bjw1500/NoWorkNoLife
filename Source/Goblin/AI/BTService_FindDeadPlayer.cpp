// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindDeadPlayer.h"
#include "BTService_CheckCanChase.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Characters/BaseCharacter.h"
#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"


void UBTService_FindDeadPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//URPGBlueprintLibrary::DebugLog(TEXT("Test"));


	APawn* OwingPawn = AIOwner->GetPawn();

	/*Owner가 플레이어면 몬스터를 찾고, 몬스터면 플레이어를 찾는다*/
	const FName TargetTag = OwingPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwingPawn, TargetTag, ActorsWithTag);

	float ClosesetDistance = TNumericLimits<float>::Max();
	AActor* ClosesetActor = nullptr;
	for (AActor* Actor : ActorsWithTag)
	{

		ABaseCharacter* BC = Cast<ABaseCharacter>(Actor);
		if(BC == nullptr)
			continue;

		if (IsValid(Actor) && IsValid(OwingPawn) &&BC->bDead && BC->bUnDead == false)
		{
			const float Distance = OwingPawn->GetDistanceTo(Actor);
			if (Distance < ClosesetDistance)
			{
				ClosesetDistance = Distance;
				ClosesetActor = Actor;
			}
		}

	}

	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosesetActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosesetDistance);
}

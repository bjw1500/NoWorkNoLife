// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Characters/BaseCharacter.h"
#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"

void UBTService_FindNearPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//URPGBlueprintLibrary::DebugLog(TEXT("Test"));


	ABaseCharacter* OwingPawn = Cast<ABaseCharacter>(AIOwner->GetPawn());

		/*Owner가 플레이어면 몬스터를 찾고, 몬스터면 플레이어를 찾는다*/
	const FName TargetTag = OwingPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwingPawn, TargetTag, ActorsWithTag);

	float ClosesetDistance = TNumericLimits<float>::Max();
	AActor* ClosesetActor = nullptr;
	for (AActor* Actor : ActorsWithTag)
	{

		if (IsValid(Actor) && IsValid(OwingPawn))
		{
			const float Distance = OwingPawn->GetDistanceTo(Actor);
			if (Distance < ClosesetDistance)
			{
				ClosesetDistance = Distance;
				ClosesetActor = Actor;
			}
		}
	}

	OwingPawn->SetCombatTarget_Implementation(ClosesetActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosesetDistance);
}

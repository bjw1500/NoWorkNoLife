// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckCanChase.h"
#include "BTService_CheckCanChase.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Characters/BaseCharacter.h"
#include "Interface/CombatInterface.h"
#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"

void UBTService_CheckCanChase::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ABaseCharacter* OwingPawn = Cast<ABaseCharacter>(AIOwner->GetPawn());
	ABaseCharacter* Target = Cast<ABaseCharacter>(UBTFunctionLibrary::GetBlackboardValueAsActor(this, TargetToFollowSelector));
	if (Target == nullptr || OwingPawn == nullptr || Target->bDead == true)
	{
		OwingPawn->SetCombatTarget_Implementation(nullptr);
		//UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, nullptr);
		return;
	}	

	float Distance = OwingPawn->GetDistanceTo(Target);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTarget, Distance);
	float CanChaseRange = UBTFunctionLibrary::GetBlackboardValueAsFloat(this, SearchRange);
	if (CanChaseRange < Distance)
	{
		OwingPawn->SetCombatTarget_Implementation(nullptr);
		//UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, nullptr);
		return;
	}
}

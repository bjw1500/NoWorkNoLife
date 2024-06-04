// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindNearItem.generated.h"

/**
 * 
 */
UCLASS()
class GOBLIN_API UBTService_FindNearItem : public UBTService_BlueprintBase
{
	GENERATED_BODY()
	
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector DiscoverItem;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector SearchRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName TargetTag = FName("Item");
};

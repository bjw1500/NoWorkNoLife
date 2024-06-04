// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_CheckBehindTarget.generated.h"

/**
 * 
 */
UCLASS()
class GOBLIN_API UBTService_CheckBehindTarget : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector TargetToFollow;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector  IsBehindTarget;
	
};

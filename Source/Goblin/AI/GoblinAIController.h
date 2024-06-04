// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GoblinAIController.generated.h"

/**
 * 
 */

 class UBehaviorTreeComponent;

UCLASS()
class GOBLIN_API AGoblinAIController : public AAIController
{
	GENERATED_BODY()
public:
	AGoblinAIController();

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
};

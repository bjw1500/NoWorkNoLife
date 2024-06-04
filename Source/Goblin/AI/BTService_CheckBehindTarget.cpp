// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckBehindTarget.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Characters/BaseCharacter.h"
#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"

void UBTService_CheckBehindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AActor* Player = UBTFunctionLibrary::GetBlackboardValueAsActor(this, TargetToFollow);
    ABaseCharacter* OwingPawn = Cast<ABaseCharacter>(AIOwner->GetPawn());

    if (Player == nullptr || OwingPawn == nullptr)
    {
            UBTFunctionLibrary::SetBlackboardValueAsBool(this, IsBehindTarget, false);
            return;
    }

    FVector PlayerForward = Player->GetActorForwardVector();
    FVector ObjectLocation = OwingPawn->GetActorLocation();
    FVector PlayerLocation = Player->GetActorLocation();

    FVector ObjectToPlayer = PlayerLocation - ObjectLocation;

    float DotProduct = FVector::DotProduct(PlayerForward, ObjectToPlayer);

    // Dot product가 양수인 경우, 객체는 플레이어의 앞쪽에 위치.

    if (DotProduct > 0)
    {
        UBTFunctionLibrary::SetBlackboardValueAsBool(this, IsBehindTarget, false);
    }
    else
    {
        UBTFunctionLibrary::SetBlackboardValueAsBool(this, IsBehindTarget, true);
    }

    return;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StoreProduct.generated.h"

class USkeletalMeshComponent;
struct FItemData;
class AItemBase;

UCLASS()
class GOBLIN_API AStoreProduct : public AActor
{
	GENERATED_BODY()

public:
	AStoreProduct();

protected:
	virtual void BeginPlay() override;

private:

};

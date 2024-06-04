// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/Item/ItemBase.h"
#include "GoblinStore.generated.h"

struct FItemData;
class AStoreProduct;

UCLASS()
class GOBLIN_API AGoblinStore : public AItemBase
{
	GENERATED_BODY()

public:
	//virtual void Tick(float DeltaTime) override;
	AGoblinStore();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void InitProduct();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> StoreDataTable;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FItemData> StoreItemData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AStoreProduct*> ProductArray;
};

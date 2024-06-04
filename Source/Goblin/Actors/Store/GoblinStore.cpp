// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Store/GoblinStore.h"
#include "Actors/Item/ItemClassInfo.h"
#include "Components/SkeletalMeshComponent.h"
#include "Actors/Store/StoreProduct.h"

AGoblinStore::AGoblinStore()
{
	//PrimaryActorTick.bCanEverTick = true;
	//Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	//RootComponent = Mesh;
}

void AGoblinStore::BeginPlay()
{
	Super::BeginPlay();
	
	if (StoreDataTable == nullptr)
	{
		return;
	}

	TArray<FName> RowName = StoreDataTable->GetRowNames();

	for (FName ItemName : RowName)
	{
		FItemData* Item = StoreDataTable->FindRow<FItemData>(ItemName, FString(""));
		StoreItemData.Add(*Item);
	}

	InitProduct();
}
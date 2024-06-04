// Fill out your copyright notice in the Description page of Project Settings.


#include "DataManager.h"
#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"
#include "Actors/Item/ItemClassInfo.h"

void UDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ItemTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, TEXT("/Script/Engine.DataTable'/Game/DataTable/DT_SaleItemTable.DT_SaleItemTable'")));
	if (IsValid(ItemTable) == false)
	{
		UGoblinBlueprintFunctionLibrary::DebugLog(TEXT("ItemTable 로드 실패. 확인해주세요."));
	}

}

FItemData UDataManager::GetItemData(int32 id)
{
	if (IsValid(ItemTable) == false)
	{
		UGoblinBlueprintFunctionLibrary::DebugLog(TEXT("ItemTable가 없습니다. 확인해주세요."));
		return FItemData();
	}

	return *ItemTable->FindRow<FItemData>(*FString::FromInt(id), TEXT(""));
}

FItemData UDataManager::GetItemDataByName(FName Name)
{
	if (IsValid(ItemTable) == false)
	{
		UGoblinBlueprintFunctionLibrary::DebugLog(TEXT("ItemTable가 없습니다. 확인해주세요."));
		return FItemData();
	}


	return *ItemTable->FindRow<FItemData>(Name, TEXT(""));
}

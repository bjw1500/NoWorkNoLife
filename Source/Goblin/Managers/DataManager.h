// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataManager.generated.h"

/**

	DataAsset vs DataTable
	https://forums.unrealengine.com/t/datatable-vs-udataasset-for-storing-large-amount-of-static-data/111622
	안정적이고 언리얼 엔진의 에셋과 결합성이 강하면 DataAsset추천.
	대신 DataTable은 작업이 편하다.


	TODO
	데이터 시트 관리 및 엑셀 파일 읽기 만들어주기
	ex)Quest 정보, NPC 대화 스크립트.

	다만 캐릭터 스폰과 관련된 클래스 데이터 시트는 GameMode에도 관리하도록 설정.
	이유는 데디 서버 개념에서 클라이언트와 서버 사이의 위치를 좀 더 명확하게 해주기 위해서임.
 */

struct FItemData;

UCLASS()
class GOBLIN_API UDataManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection);

	UFUNCTION(BlueprintPure)
	FItemData GetItemData(int32 id);

	UFUNCTION(BlueprintPure)
	FItemData GetItemDataByName(FName Name);

public:

	UPROPERTY(EditAnyWhere, BlueprintReadOnly);
	TObjectPtr<UDataTable> ItemTable;
	
};

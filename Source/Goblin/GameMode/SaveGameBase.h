// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameBase.generated.h"

/**

	데이터를 저장할 세이브 데이터 정의.

 */
UCLASS()
class GOBLIN_API USaveGameBase : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString SlotName = FString();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 SlotIndex = 0;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Money = 100;

	//이번 라운드에 제물로 바친 영혼의 양
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 SellMoney = 0;

	//살아남은 날
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 SurvivalDate = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 RemainDate = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Quota = 0;

};

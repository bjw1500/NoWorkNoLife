// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/GameManager.h"
#include "Actors/Item/ItemClassInfo.h"
#include "Net/UnrealNetwork.h"
#include "GameMode/GoblinGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UGameManager::Init()
{
	Super::Init();
}

void UGameManager::Shutdown()
{
	Super::Shutdown();
}

void UGameManager::StartGameInstance()
{
	Super::StartGameInstance();
}

void UGameManager::SetMoney(int32 Money)
{
	Server_Money = Money;
	UpdateGameStateUI();
}

void UGameManager::SetDate(int32 Date)
{
	Server_Date = Date;
	UpdateGameStateUI();
}

void UGameManager::SetLevel(int32 Level)
{
	Server_Level = Level;
	UpdateGameStateUI();
}

void UGameManager::SetSellMoney(int32 Value)
{
	Server_SellMoney = Value;
	UpdateGameStateUI();

}

void UGameManager::SetSurvivalDate(int32 Value)
{
	Server_SurvivalDate = Value;
	UpdateGameStateUI();
}

void UGameManager::SetInventorySlotCount(int32 Value)
{
	UISlotindex = Value;
	UpdateGameStateUI();
}
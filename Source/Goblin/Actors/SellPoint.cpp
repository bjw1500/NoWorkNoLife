// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SellPoint.h"
#include "Actors/Item/ItemBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PlayerCharacter.h"
#include "GameMode/GoblinGameStateBase.h"

ASellPoint::ASellPoint()
{
	bReplicates = true;
}

void ASellPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASellPoint, PutItem);
	DOREPLIFETIME_CONDITION(ASellPoint, RandKeyList, COND_SimulatedOnly);
	DOREPLIFETIME(ASellPoint, Time);
	DOREPLIFETIME(ASellPoint, CheckIndex);
}

void ASellPoint::BeginPlay()
{
	Super::BeginPlay();
}

void ASellPoint::CheckItems(AActor* OverlapActor)
{
	AItemBase* Item = Cast<AItemBase>(OverlapActor);

	if (Item != nullptr)
	{
		PutItem.Add(Item);
	}
}

void ASellPoint::CreateRandString_Implementation()
{
	if (!RandKeyList.IsEmpty())
	{
		return;
	}

	SuccessFlag = false;

	for (int i = 0; i <= 6; i++)
	{
		int32 RandomIndex = FMath::RandRange(0, KeyList.Num() - 1);

		RandKeyList.Add(KeyList[RandomIndex]);
	}
	Time = MaxTime;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASellPoint::AdvanceTimer, 0.1f, true);
}

bool ASellPoint::Server_CheckAZPattern_Validate(const FString& Input)
{
	if (KeyList.Contains(Input))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ASellPoint::Server_CheckAZPattern_Implementation(const FString& Input)
{
	if (RandKeyList.IsEmpty())
	{
		return;
	}

	if (CheckIndex >= RandKeyList.Num() - 1)
	{
		//CheckItemOnSellPoint();
		CheckIndex = 0;
		//Server_AddMoney();

		//for (AItemBase* Item : PutItem)
		//{
		//	if(IsValid(Item) == false)
		//		continue;
		//	Item->DissolveDestroy();
		//}
		//PutItem.Empty();
		SuccessFlag = true;
		SuccessEvent();
		Client_AZPatternSuccess();
	}

	if (RandKeyList[CheckIndex] == Input)
	{
		Client_SetSuccessKeyUI();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d : %s"), CheckIndex, *Input));
		CheckIndex++;
	}
	else
	{
		CheckIndex = 0;
		Client_SetFailKeyUI();
	}
}

void ASellPoint::Server_AddMoney_Implementation()
{
	AGoblinGameStateBase* GameState = Cast<AGoblinGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	checkf(GameState, TEXT("SellPoint : GameManager is Null"));

	int CalcMoney = 0;
	for (auto& Item : PutItem)
	{
		if(IsValid(Item) == false)
			return;

		if(Item->ItemData.ItemType == EItemType::SaleItem)
			CalcMoney +=Item->ItemData.Price;
		Item->DissolveDestroy();
	}
	PutItem.Empty();
	GameState->Server_AddMoney(CalcMoney);
}

void ASellPoint::Client_AZPatternSuccess_Implementation()
{
	RemoveInputBinding();
	GetWorldTimerManager().ClearTimer(TimerHandle);

	//TODO : 성공했을때 아이템 가져가기
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("is Success"));
}

void ASellPoint::Server_AZPatternFail_Implementation()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	if (Player == nullptr)
	{
		return;
	}

	if(SuccessFlag == true)
		return;

	FailEvent();
	Player->Die();
}

void ASellPoint::AdvanceTimer()
{
	Time -= 0.1f;

	if (Time <= 0.0f)
	{
		Server_AZPatternFail();
		RemoveInputBinding();
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}

void ASellPoint::Client_SetSuccessKeyUI_Implementation()
{
	SetSuccessKeyUI();
}

void ASellPoint::Client_SetFailKeyUI_Implementation()
{
	SetFailKeyUI();
}

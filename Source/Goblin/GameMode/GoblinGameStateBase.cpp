// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GoblinGameStateBase.h"
#include "GameMode/GoblinGameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/GameManager.h"
#include "Math/UnrealMathUtility.h"

void AGoblinGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	

	if (QuotaData == nullptr)
	{
		return;
	}
	Server_SetVeriable();
	DataRowName = QuotaData->GetRowNames();
}

void AGoblinGameStateBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGoblinGameStateBase, Level);
	DOREPLIFETIME(AGoblinGameStateBase, RemainDate);
	DOREPLIFETIME(AGoblinGameStateBase, SurvivalDate);
	DOREPLIFETIME(AGoblinGameStateBase, Money);
	DOREPLIFETIME(AGoblinGameStateBase, SellMoney);
	DOREPLIFETIME(AGoblinGameStateBase, RandomRate);
	DOREPLIFETIME(AGoblinGameStateBase, InventorySlotCount);
}

void AGoblinGameStateBase::Server_SetVeriable_Implementation()
{
	UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameManager == nullptr)
	{
		return;
	}

	SetMoney(GameManager->GetMoney());
	SetDate(GameManager->GetDate());
	SetLevel(Level = GameManager->GetLevel());
}

int32 AGoblinGameStateBase::GetQuotaMoney()
{
	checkf(QuotaData, TEXT("QuotaData is null"));
	FName RowName = FName(FString::FromInt(Level));
	FQuota* Quota = QuotaData->FindRow<FQuota>(RowName, FString(""));
	return Quota->Money;
}

void AGoblinGameStateBase::Server_AddDateAndCheckLevel_Implementation(const FString& MapURL)
{
	UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameManager == nullptr)
	{
		return;
	}

	int32 CurrentDate = GameManager->GetDate();
	int32 CurrentSurvivalDate = GameManager->GetSurvivalDate();

	SetDate(CurrentDate - 1);
	SetSurvivalDate(CurrentSurvivalDate + 1);
	//GameManager->SetDate(CurrentDate - 1);
	//GameManager->SetSurvivalDate(CurrentSurvivalDate + 1);

	AGoblinGameModeBase* GM = Cast<AGoblinGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));


	if (GameManager->GetDate() <= 0)
	{
		//할당량을 채웠는지 확인.

		if (GameManager->GetSellMoney() >= GetQuotaMoney())
		{
			int32 NextLevel = GameManager->GetLevel();
			SetLevel(NextLevel +1);
			SetSellMoney(0);
			SetDate(3);

			if (GM)
			{
				//할당량 이벤트를 해주자.
				GM->SuccessQuota();
			}
		}
		else
		{
			if (GM)
			{
				//SetSellMoney(0);
				//SetDate(4);
				//SetSurvivalDate(0);
				GM->FailQuota();
			}
		}
	}
}


bool AGoblinGameStateBase::CheckMoney(int32 Price)
{
	if (Money >= Price)
	{
		return true;
	}
	return false;
}

void AGoblinGameStateBase::LoadGameManagerData()
{
	UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameManager)
	{
		Money = GameManager->GetMoney();
		SellMoney = GameManager->GetSellMoney();
		Level = GameManager->GetLevel();
		RemainDate = GameManager->GetDate();
		SetSurvivalDate(GameManager->GetSurvivalDate());
		SetInventorySlotCount_C(GameManager->UISlotindex);
	}

}

void AGoblinGameStateBase::SetMoney(int32 Value)
{
	Money = Value;
	UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameManager)
	{
		GameManager->SetMoney(Value);
	}

	UpdateUI();
}

void AGoblinGameStateBase::SetDate(int32 Value)
{
	RemainDate = Value;
	RandomRate = FMath::RandRange(0.3, 1.2);
	UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameManager)
	{
		GameManager->SetDate(Value);
	}
	UpdateUI();


	AGoblinGameModeBase* GM = Cast<AGoblinGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->AllClearEvent();
	}

}

void AGoblinGameStateBase::SetLevel(int32 Value)
{
	Level = Value;
	UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameManager)
	{
		GameManager->SetLevel(Value);
	}
	UpdateUI();
}

void AGoblinGameStateBase::SetSellMoney(int32 Value)
{
	SellMoney = Value;
	UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameManager)
	{
		GameManager->SetSellMoney(Value);
	}
	UpdateUI();
}

void AGoblinGameStateBase::SetMoney_C(int32 Value)
{
	Money = Value;
	UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameManager)
	{
		GameManager->SetMoney(Value);
	}

	UpdateUI();
}

void AGoblinGameStateBase::SetSurvivalDate(int32 Value)
{
	SurvivalDate = Value;
	UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameManager)
	{
		GameManager->SetSurvivalDate(Value);
	}
	UpdateUI();

	
	if (SurvivalDate >= 60)
	{
		AGoblinGameModeBase* GM = Cast<AGoblinGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			GM->GameClearEvent();
		}

	}

}

void AGoblinGameStateBase::SetInventorySlotCount_C(int32 Value)
{
	InventorySlotCount = Value;
	UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameManager)
	{
		GameManager->SetInventorySlotCount(Value);
	}
	UpdateUI();
}


void AGoblinGameStateBase::Rep_Level(int32 OldLevel)
{
	Rep_LevelEvent(OldLevel);
}

void AGoblinGameStateBase::Rep_RemainDate(int32 OldDate)
{
	Rep_RemainDateEvent(OldDate);
}

void AGoblinGameStateBase::Rep_Money(int32 OldMoney)
{
	Rep_MoneyEvent(OldMoney);
}

void AGoblinGameStateBase::Rep_InventorySlotCount(int32 OldCount)
{
	Rep_InventorySlotCountEvent(OldCount);
}

void AGoblinGameStateBase::Server_AddMoney_Implementation(int32 Value, bool IsSell)
{
	AGoblinGameModeBase* GoblinGameMode = Cast<AGoblinGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GoblinGameMode == nullptr || GameManager == nullptr)
	{
		return;
	}

	// 아이템 판매
	if (IsSell)
	{
		int32 NextMoney = Value * RandomRate + GameManager->GetMoney();;
		SetMoney(NextMoney);

		int32  NextSellMoney = GameManager->GetSellMoney() + Value * RandomRate;
		SetSellMoney(NextSellMoney);
	}
	// 아이템 구매
	else
	{
		int NextMoney = GameManager->GetMoney() - Value;
		SetMoney(NextMoney);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Money : %d"), Money));

}

bool AGoblinGameStateBase::Server_AddMoney_Validate(int32 Value, bool IsSell)
{
	if (Value >= 0)
	{
		return true;
	}
	return false;
}

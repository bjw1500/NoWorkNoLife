// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GoblinGameStateBase.generated.h"

struct FItemData;

USTRUCT(Atomic, BlueprintType)
struct FQuota : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Money;
};

/**
 * 
 */
UCLASS()
class GOBLIN_API AGoblinGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay();
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const;

	/** 서버가 가지고 있는데이터 스테이트 변수에 복사 */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SetVeriable();

	/** 할당량 데이터 가져오기 */
	UFUNCTION(BlueprintCallable)
	int32 GetQuotaMoney();

	/** (서버) 3일 지났는지 체크 */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_AddDateAndCheckLevel(const FString& MapURL);

	UFUNCTION(BlueprintCallable)
	bool CheckMoney(int32 Price);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_AddMoney(int32 Value, bool IsSell = true);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BuyItemInStoreEvent(FItemData ItemData);

	/** 할당량 데이터 가져오기 */
	UFUNCTION(BlueprintCallable)
	void LoadGameManagerData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> QuotaData;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> DataRowName;

private:

		/** 영혼을 팔아서 얻은 돈 */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, ReplicatedUsing = "Rep_Money", meta = (AllowPrivateAccess = "true"))
	int32 Money;

	/*할당량을 위해 모으는 중인 돈*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated, meta = (AllowPrivateAccess = "true"))
	int32 SellMoney;

		/** 할당량 레벨 */
	UPROPERTY(EditAnywhere, ReplicatedUsing = "Rep_Level", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Level;

		/** 3일 날짜 카운트하기 위한 변수 */
	UPROPERTY(EditAnywhere, ReplicatedUsing = "Rep_RemainDate", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 RemainDate;

	/** 할당량 날마다 랜덤 설정 */
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float RandomRate;

	/** 살아남은 날수 */
	UPROPERTY(EditAnywhere, ReplicatedUsing = "Rep_RemainDate", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 SurvivalDate;

	UPROPERTY(EditAnywhere, ReplicatedUsing = "Rep_InventorySlotCount", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 InventorySlotCount;

public:

	void SetMoney(int32 Value);
	void SetDate(int32 Value);
	void SetLevel(int32 Value);
	void SetSellMoney(int32 Value);

	UFUNCTION(BlueprintCallable)
	void SetMoney_C(int32 Value);
	void SetSurvivalDate(int32 Value);

	UFUNCTION(BlueprintCallable)
	void SetInventorySlotCount_C(int32 Value);

	int32 GetMoney() { return Money; }
	int32 GetDate() { return RemainDate; }
	int32 GetLevel() { return Level; }
	int32 GetSellMoney(){return SellMoney;}
	int32 GetSurvivalDate() { return SurvivalDate; }
	int32 GetInventorySlotCount() {return InventorySlotCount;}
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateUI();

	UFUNCTION()
	void Rep_Level(int32 OldLevel);
	UFUNCTION(BlueprintImplementableEvent)
	void Rep_LevelEvent(int32 OldLevel);


	UFUNCTION()
	void Rep_RemainDate(int32 OldDate);
	UFUNCTION(BlueprintImplementableEvent)
	void Rep_RemainDateEvent(int32 OldDate);


	UFUNCTION()
	void Rep_Money(int32 OldMoney);
	UFUNCTION(BlueprintImplementableEvent)
	void Rep_MoneyEvent(int32 OldMoney);

	UFUNCTION()
	void Rep_InventorySlotCount(int32 OldCount);
	UFUNCTION(BlueprintImplementableEvent)
	void Rep_InventorySlotCountEvent(int32 OldCount);
};

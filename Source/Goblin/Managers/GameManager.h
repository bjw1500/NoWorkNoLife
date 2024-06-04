// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
//#include "AdvancedFriendsGameInstance.h"
#include "AdvancedFriendsGameInstance.h"
#include "GameManager.generated.h"

/**
 *
 */
UCLASS()
class GOBLIN_API UGameManager : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void StartGameInstance() override;

private:
	
	/** (서버) 할당량 레벨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Server_Level = 0;

	/** (서버)3일 날짜 카운트하기 위한 변수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Server_Date = 1;

	/** (서버) 돈*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Server_Money = 100000;

	/* 할달량을 위해 판매한 돈*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Server_SellMoney = 0;

	/** 지금까지 살아남은 날수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Server_SurvivalDate = 0;

public:

	void SetMoney(int32 Money);
	void SetDate(int32 Date);
	void SetLevel(int32 Level);
	void SetSellMoney(int32 Value);
	void SetSurvivalDate(int32 Value);

	int32 GetMoney() {return Server_Money;}
	int32 GetDate(){return Server_Date;}
	int32 GetLevel(){return Server_Level;}
	int32 GetSellMoney(){return Server_SellMoney;}
	int32 GetSurvivalDate(){return Server_SurvivalDate;}

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateGameStateUI();

public:

	/** Inventory 슬롯*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 UISlotindex = 3;

	UFUNCTION(BlueprintCallable)
	void SetInventorySlotCount(int32 Value);

	//옵션
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MouseSensivity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GammaValue = 2.2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MasterSoundValue = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool VoiceEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AlwaysOnVoice = true;



protected:

private:

	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "GameFramework/Actor.h"
#include "SellPoint.generated.h"


class AItemBase;

UCLASS()
class GOBLIN_API ASellPoint : public AItemBase
{
	GENERATED_BODY()

public:
	ASellPoint();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	/*
	* 올려져있는 아이템 배열에 추가
	*/
	UFUNCTION(BlueprintCallable)
	void CheckItems(AActor* OverlapActor);

	/*
	* (서버) 아재패턴 시작시 렌덤으로 문자열 생성
	*/
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void CreateRandString();

	/*
	* (서버)입력받은 키와 랜덤으로 생성된 키 비교 후 서버에 완료 요쳥
	*/
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_CheckAZPattern(const FString& Input);

	/*
	* (서버) 아재패턴 성공했을때 서버에에서 함수 호출이 되면 클라에 적용
	*	확인된 아이템들의 가치를 측정하고, 이벤트가 끝나면 파괴한다.
	*/
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_AddMoney();
	/*
	* 클라 아재패턴 성공했을때 서버에에서 함수 호출이 되면 클라에 적용
	*/
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_AZPatternSuccess(); 

	/*
	* (서버) 아재패턴 실패시 서버에서 호출
	*/
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_AZPatternFail();

	/*
	* 인폿 바인딩 관련
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInputBiding();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RemoveInputBinding();

	/*
	* (클라) 아재패턴 성공시 서버에서 호출하면 클라에 적용
	*/
	UFUNCTION(Client, Reliable)
	void Client_SetSuccessKeyUI();
	/*
	* 아재패턴 성공시 클라에서 호출할 함수를 블루프린트에서 구현
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void SetSuccessKeyUI();

	/*
	* (클라) 아재패턴 실패시 서버에서 호출하면 클라에 적용
	*/
	UFUNCTION(Client, Reliable)
	void Client_SetFailKeyUI();
	/*
	* 아재패턴 실패시 클라에서 호출할 함수를 블루프린트에서 구현
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void SetFailKeyUI();
	

	/*
		패턴 성공시 올라간 아이템 확인.
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void CheckItemOnSellPoint();


	/*
		성공 이벤트
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void SuccessEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void FailEvent();

private:
	// 올려놓은 아이템
	UPROPERTY(BlueprintReadOnly, Replicated, meta = (AllowPrivateAccess = "ture"))
	TArray<AItemBase*> PutItem;

	TArray<FString> KeyList = { "Q", "W", "E" , "R" , "A" , "S" , "D" , "F" };

	UPROPERTY(BlueprintReadOnly, Replicated, meta = (AllowPrivateAccess = "ture"))
	TArray<FString> RandKeyList;

	UPROPERTY(BlueprintReadOnly, Replicated, meta = (AllowPrivateAccess ="ture"))
	int32 CheckIndex;

	/*
	* 타이머
	*/
	UPROPERTY(BlueprintReadOnly, Replicated, meta = (AllowPrivateAccess = "ture"))
	float Time;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "ture"))
	float MaxTime = 10.f;

	FTimerHandle TimerHandle;
	void AdvanceTimer();

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "ture"))
	bool SuccessFlag = false;
};

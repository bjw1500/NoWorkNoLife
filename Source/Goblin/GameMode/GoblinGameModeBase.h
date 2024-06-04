// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GoblinGameModeBase.generated.h"


class UCharacterClassInfo;
class APlayerController;

UCLASS()
class GOBLIN_API AGoblinGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void GetSeamlessTravelActorList(bool bToEntry, TArray< class AActor* >& ActorList) override;


	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeMoneyUI();

	UFUNCTION(BlueprintImplementableEvent)
	void AddDeadPlayer(AController* Player);

	UFUNCTION(BlueprintImplementableEvent)
	void AddTravelActorList(TArray<AActor*>& ActorList);

		//SaveData
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SaveData(const FString& SlotName);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void LoadData(const FString& SlotName);

	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FailQuota();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SuccessQuota();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AllClearEvent();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GameClearEvent();

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Gobline | GameMode", meta = (AllowPrivateAccess= "true"))
	TArray<AActor*> TestList;

};

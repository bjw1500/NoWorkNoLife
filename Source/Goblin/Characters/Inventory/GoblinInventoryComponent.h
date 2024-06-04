// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Actors/Item/ItemClassInfo.h"
#include "GoblinInventoryComponent.generated.h"

class AGoblinHUD;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GOBLIN_API UGoblinInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGoblinInventoryComponent();

	// 혹시몰라
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintCallable, Category = "Gobliln | Inventory Component")
	void SetUpItemData(FItemData ItemData);

	/** (서버) 아이템 주웠을때 인벤토리 추가 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Gobliln | Inventory Component")
	void Server_AddInventoryArray(const FItemData Data, bool IsHeldItem = false);
	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Gobliln | Inventory Component")
	void Client_AddInventoryArray(const FItemData Data);

	/** (서버) 아이템 놓았을때 인벤토리 삭제 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Gobliln | Inventory Component")
	void Server_RemoveItemDataToDrop();
	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Gobliln | Inventory Component")
	void Client_RemoveItemDataToDrop(const FItemData Data);

	/** (서버) 죽었을때 인벤토리 비워주기 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Gobliln | Inventory Component")
	void Server_ClearInventoryItemData();

	/** (서버) 슬롯 인덱스 설정 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetSlotIndex(const FKey& Input);
	/** 클라 인덱스 설정 */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_SetSlotIndex(const int32& Input);


	UFUNCTION(BlueprintCallable)
	float GetDuration();

	/** 인벤토리 게임인스턴스에 저장하기 */
	UFUNCTION(BlueprintCallable)
	TArray<FItemData> GetInventoryDatas() { return Inventory; }

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void TestCheckItems();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Goblin | Inventory", meta = (AllowPrivateAccess = "True"))
	int32 SlotIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Goblin | Inventory", meta = (AllowPrivateAccess = "True"))
	int32 SlotMaxIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Goblin | Inventory", meta = (AllowPrivateAccess = "True"))
	FItemData PickUpItemInfo;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Goblin | Inventory", meta = (AllowPrivateAccess = "True"))
	TArray<FItemData> Inventory;
};

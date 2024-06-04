// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;
class AItemBase;
class UGoblinInventoryComponent;
class IObjectInterface;
class UBehaviorTree;
class AGoblinAIController;

UCLASS()
class GOBLIN_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:

	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
	virtual void OnRep_PlayerState() override;

	/*CombatInterface*/
	virtual int32 GetLevel() override;
	
	UFUNCTION(BlueprintCallable)
	void DieRightNow();

	virtual void Die() override;
	UFUNCTION(BlueprintImplementableEvent, Category = "Goblin")
	void DieEvent();

	/*PlayerInterface*/
	/*
		Try -> Mutilcast
		서버 속성으로 서버에서 실행해야할 함수를 구현하고,
		그 다음 모든 클라이언트에서 적용할 Multicast 함수를 만든다.
	*/
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Goblin | PlayerInterface")
	void TryPickUpItem(AItemBase* Item, bool IsSwapCheck = false);
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Goblin | PlayerInterface")
	void TryDropItem();
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Goblin | PlayerInterface")
	void TryUseItem();
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Goblin | PlayerInterface")
	void TryPutInItem();

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Goblin | PlayerInterface")
	void EquipItem(AItemBase* Item);		//아이템 장비
	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Goblin | PlayerInterface")
	void DropItem(AItemBase* Item);						//아이템 해제

	//시체 줍기
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Goblin | PlayerInterface")
	void TryPickUpDeadBody(ABaseCharacter* DeadBody, bool IsSwapCheck = false);
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Goblin | PlayerInterface")
	void Multicast_PickUpDeadBody(ABaseCharacter* DeadBody, bool IsSwapCheck = false);
		UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Goblin | PlayerInterface")
	void Multicast_DropDeadBody();


	/*
	*	인벤토리
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Goblin | PlayerInventory")
	void SetinventoryWidget();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Goblin | PlayerInventory")
	void GetItemData(const FItemData Data);
	UFUNCTION(BlueprintImplementableEvent, Category = "Goblin | PlayerInventory")
	void PickUpItemData(const FItemData Data);
	UFUNCTION(BlueprintImplementableEvent, Category = "Goblin | PlayerInventory")
	void DropItemData(const FItemData Data);
	UFUNCTION(BlueprintImplementableEvent)
	void SetSlotIndexEvent(int32 Index);

	/*
		추격당할 때 실행되는 함수
	*/

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Goblin")
	void IncreaseChasingMonsterCount(AEnemyCharacter* Enemy);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Goblin")
	void DecreaseChasingMonsterCount(AEnemyCharacter* Enemy);

	UFUNCTION()
	UGoblinInventoryComponent* GetInventoryComponent() { return InventoryComponent; }

private:
	virtual void InitAbilityActorInfo() override;

private:


	UPROPERTY(BlueprintReadOnly, Category = "Goblin | Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGoblinInventoryComponent> InventoryComponent;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "Goblin")
	int32  ChasingMonsterCount;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UCameraComponent* FollowCamera;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class USpringArmComponent* SpringArm;
};

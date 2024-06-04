// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GoblinPlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
 class UAbilitySystemComponent;
 class UGoblinInputConfig;
 class UGoblinAbilitySystemComponent;
 struct FGameplayTag;
 class AItemBase;
 class ABaseCharacter;
 class IObjectInterface;
 class UGameManager;
 class UGoblinInventoryComponent;
 class UUserWidget;

 UENUM(BlueprintType)
enum class EPlayeCharacterState : uint8
{
	Alive,
	Dead,
	Escape,
};

 DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopActionBar);

UCLASS()
class GOBLIN_API AGoblinPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AGoblinPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UGoblinAbilitySystemComponent* GetASC();
	UFUNCTION(BlueprintCallable)
	UGoblinInventoryComponent* GetInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void FindNearObject();														//플레이어의 시선이 닿는 곳에 아이템이 있는지 확인하는 함수
	UFUNCTION(BlueprintCallable)
	void FindNearEnemy();
	void ScanNearItem();														//플레이어의 주변에 아이템이 있는지 확인하는 함수.
	void PickUpItem();															//아이템을 줍는 함수

	UFUNCTION(BlueprintCallable)
	void DropItem();
	void EquipItem();

	//UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Goblin | PlayerInterface")
	//void TryHighlightObject(IObjectInterface* Object);
	//UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Goblin | PlayerInterface")
	//void TryUnHighlightObject(IObjectInterface* Object);


	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_InteractItem(AItemBase* Item);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartActionBar(AItemBase* Item);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StopActionBar();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StopActionBar2();

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FStopActionBar OnStopActionBar;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowScript(AItemBase* Item);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CloseScript();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void BuyItemInStore(int32 Price, FItemData ItemData);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_BuyItemSuccess(FItemData ItemData);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BuyItemSuccessEvent(FItemData ItemData);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_ShortOfSoul();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShortOfSoulEvent();

	void InteractItem();
	void Jump();

public:

	UPROPERTY()
	TObjectPtr<AActor> ThisObject;
		UPROPERTY()
	TObjectPtr<AActor> LastObject;
		UPROPERTY()
	TObjectPtr<ABaseCharacter> ThisCharacter;
		UPROPERTY()
	TObjectPtr<ABaseCharacter>  LastCharacter;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetThisObject();

	UPROPERTY(EditAnywhere, Replicated,  Category = "Goblin | ObjectInterface")
	float DetectedTime = 0.f;;

	UPROPERTY(EditAnywhere, Replicated, Category = "Goblin | ObjectInterface")
	float WatchDuration = 0.f;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated, Category = "Goblin | PlayerState")
	bool EscapeSuccess = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Goblin | PlayerState")
	EPlayeCharacterState PlayerCharacterState = EPlayeCharacterState::Alive;


	//입력 딜레이용 타이머
	FTimerHandle InputDelayTimer;
	bool CanInput = true;

	/*
		입력
	*/
	UPROPERTY(EditAnywhere, Category = "Goblin | Input")
	TObjectPtr<UInputMappingContext> PlayerInputContext;

	UPROPERTY(EditAnywhere, Category = "Goblin | Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Goblin | Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Goblin | Input")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditAnywhere, Category = "Goblin | Input")
	TObjectPtr<UInputAction> CtrlAction;

	UPROPERTY(EditAnywhere, Category = "Goblin | Input")
	TObjectPtr<UInputAction> SpaceAction;

	UPROPERTY(EditAnywhere, Category = "Goblin | Input")
	TObjectPtr<UInputAction> PickUpAction;

	UPROPERTY(EditAnywhere, Category = "Goblin | Input")
	TObjectPtr<UInputAction> DropAction;

	UPROPERTY(EditAnywhere, Category = "Goblin | Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category = "Goblin | Input")
	TObjectPtr<UGoblinInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UGoblinAbilitySystemComponent> GoblinAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UGoblinInventoryComponent> InventoryComponent;

	virtual void Move(const struct FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& Value);
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;
	virtual void AbilityInputTagPressed(FGameplayTag InputTag);
	virtual void AbilityInputTagReleased(FGameplayTag InputTag);
	virtual void AbilityInputTagHeld(FGameplayTag InputTag);


	UGameManager* GameManager;

};

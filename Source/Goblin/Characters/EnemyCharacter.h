// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Interface/EnemyInterface.h"
#include "UI/GoblinWidgetController.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */

 struct FGameplayTag;
 class UBehaviorTree;
 class AGoblinAIController;
 class AItemBase;
 class APlayerController;
 class AGoblinPlayerController;
 class APlayerCharacter;

UCLASS()
class GOBLIN_API AEnemyCharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()
	public:
		AEnemyCharacter();

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitDefaultAttributes() const override;

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Goblin | PlayerInterface")
	void Server_PickUpItem(AItemBase* Item);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Goblin | PlayerInterface")
	void MulticastPickUpItem(AItemBase* Item);

	//시체 줍기
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Goblin | PlayerInterface")
	void TryPickUpDeadBody(ABaseCharacter* DeadBody, bool IsSwapCheck = false);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Goblin | PlayerInterface")
	void MulticastPickUpDeadBody(ABaseCharacter* DeadBody, bool IsSwapCheck = false);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Goblin | MonsterInterface")
	void Server_DropItem();			
	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Goblin | MonsterInterface")
	void MulticastDropItem();

	virtual void	SetCombatTarget_Implementation(AActor* InCombatTarget);


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void IsChasing(APlayerCharacter* Target);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GiveUpChase(APlayerCharacter* Target);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ScaryBlurScreen(APlayerCharacter* Target, float BlurValue);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RecoveryScreen(APlayerCharacter* Target);


public:

	/*ICombatInterface*/
	virtual int32 GetLevel() override;
	virtual void Die() override;

	/*IObjectInterface*/
	virtual void HighlightObject(AGoblinPlayerController* Player) override;
	virtual void UnHighlightObject(AGoblinPlayerController* Player) override;
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Goblin | PlayerInterface")
	virtual void Server_HighlightObject(AGoblinPlayerController* Player);
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Goblin | PlayerInterface")
	virtual void Server_UnHighlightObject(AGoblinPlayerController* Player);


	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintAssignable)
	FOnDynamicAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDynamicAttributeChangedSignature OnMaxHealthChanged;

	FOnAttributeChangedSignature OnSearchRangeChanged;
	FOnAttributeChangedSignature OnHearingRangeChanged;
	FOnAttributeChangedSignature OnAttackSpeedChanged;

	/*
		AI
	*/

	UPROPERTY(EditDefaultsOnly, Category = "Goblin | Item")
	TSubclassOf<AItemBase> MonsterWeapon;

	UPROPERTY(EditAnywhere, Category = "Goblin | AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AGoblinAIController> GoblinAIController;

	//문 여는데 필요한 시간.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goblin | AI")
	float OpenDoorTime = 3.f;

	UPROPERTY()
	FTimerHandle TimerHandle;
};

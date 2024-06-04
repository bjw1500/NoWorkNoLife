// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GoblinPlayerState.generated.h"

/**
 * 
 */

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class GOBLIN_API AGoblinPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AGoblinPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps);

	virtual UAbilitySystemComponent*				GetAbilitySystemComponent() const override;
	UAttributeSet*														GetAttributeSet() const {return AttributeSet;}
	int32																			GetPlayerLevel() const {return Level;}

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

private:
	UPROPERTY(VisibleAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;;

	UPROPERTY(VisibleAnywhere, Category = "GAS")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32	Level = 1;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoblinWidgetController.h"
#include "GameplayEffect.h"
#include "InGameWidgetController.generated.h"


/**
 * 
 */


UCLASS(BlueprintType, Blueprintable)
class GOBLIN_API UInGameWidgetController : public UGoblinWidgetController
{
	GENERATED_BODY()
public:

	virtual void BroadCastInitalValues() override;
	virtual void BindCallbacksToDependencies() override;

	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void StaminaChanged(const FOnAttributeChangeData& Data) const;
	void MaxStaminaChanged(const FOnAttributeChangeData& Data) const;

	UPROPERTY(BlueprintAssignable, Category = "Goblin | Attributes")
	FOnDynamicAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Goblin | Attributes")
	FOnDynamicAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Goblin | Attributes")
	FOnDynamicAttributeChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Goblin | Attributes")
	FOnDynamicAttributeChangedSignature OnMaxStaminaChanged;
};

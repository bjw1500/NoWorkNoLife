// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidgetController.h"
#include "GAS/GoblinAttributeSet.h"
#include "GAS/GoblinAbilitySystemComponent.h"

void UInGameWidgetController::BroadCastInitalValues()
{
	UGoblinAttributeSet* GoblinAttributeSet = CastChecked<UGoblinAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(GoblinAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(GoblinAttributeSet->GetMaxHealth());
	OnStaminaChanged.Broadcast(GoblinAttributeSet->GetStamina());
	OnMaxStaminaChanged.Broadcast(GoblinAttributeSet->GetMaxStamina());
}

void UInGameWidgetController::BindCallbacksToDependencies()
{
	/*
	현재 이 함수가 실행되는 부분은 HUD::GetInGameWidgetController 부분에서
	WidgetController가 생성될때 실행이 되고 있다.
*/


/*
	GetGameplayAttributeValueChangeDelegate
	코드를 파고들어가면, 델리게이트가 찾아봤을 때 없을 경우 즉석에서 만들어서 넣어준다는걸 알 수 있다.
*/
	UGoblinAttributeSet* GoblinAttributeSet = CastChecked<UGoblinAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GoblinAttributeSet->GetHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GoblinAttributeSet->GetMaxHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GoblinAttributeSet->GetStaminaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnStaminaChanged.Broadcast(Data.NewValue);
			});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GoblinAttributeSet->GetMaxStaminaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnMaxStaminaChanged.Broadcast(Data.NewValue);
			});
}


void UInGameWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UInGameWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UInGameWidgetController::StaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnStaminaChanged.Broadcast(Data.NewValue);
}

void UInGameWidgetController::MaxStaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxStaminaChanged.Broadcast(Data.NewValue);
}


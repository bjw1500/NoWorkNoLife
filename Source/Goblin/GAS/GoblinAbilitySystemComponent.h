// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GoblinAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);

/**
 * 
 */
UCLASS()
class GOBLIN_API UGoblinAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	
	void AbilityActorInfoSet();
	
	/*AbilitySystemEffect를 적용하는 함수*/
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	/*넘겨 받은 GameAbility 목록을 AbilitySystemComponent에 등록해준다. */
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>> StartupAbilities);
	FGameplayAbilitySpecHandle AddCharacterAbility(const  TSubclassOf<UGameplayAbility> StartupAbility);
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);


public:
	FEffectAssetTags EffectAssetTags;
};

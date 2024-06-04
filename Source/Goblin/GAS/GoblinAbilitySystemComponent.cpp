// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinAbilitySystemComponent.h"
#include "Ability/BaseGameplayAbility.h"
#include "Interface/CombatInterface.h"

void UGoblinAbilitySystemComponent::AbilityActorInfoSet()
{
	/*
* OnGameplayEffectAppliedDelegateToSelf.
	GameplayEffect가 적용될 때마다 일어나는 델리게이트.
*/
OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UGoblinAbilitySystemComponent::ClientEffectApplied);
}

void UGoblinAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//Gameplay Tag는 흥미롭게도 따로 전용 데이터 컨테이너가 존재한다.
	//그렇기에 GameEffect 정보가 담긴 Spec 구조체에서 Tag 정보를 가져올 때는 
	//따로 TagContainer를 준비해서 받아야 한다.
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	/*적용된 효과의 태그들을 모아서 델리게이트와 연결된 함수들에게 브로드캐스트 해준다*/
	EffectAssetTags.Broadcast(TagContainer);
}

void UGoblinAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>> StartupAbilities)
{

	ICombatInterface* Interface = Cast<ICombatInterface>(GetAvatarActor());
	for (const TSubclassOf<UGameplayAbility> Class : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Class, Interface->GetLevel());
		if(const UBaseGameplayAbility* BaseAbility = Cast<UBaseGameplayAbility>(AbilitySpec.Ability))
		{
			//DynamicAbilityTags은 런타임중에 Tag 추가 및 제거가 가능하다.
			AbilitySpec.DynamicAbilityTags.AddTag(BaseAbility->StartupInputTag);
			AbilitySpec.InputID = 1;
			GiveAbility(AbilitySpec);
		}
	}
}

FGameplayAbilitySpecHandle UGoblinAbilitySystemComponent::AddCharacterAbility(const TSubclassOf<UGameplayAbility> StartupAbility)
{
	
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(StartupAbility, 1);
	if (const UBaseGameplayAbility* BaseAbility = Cast<UBaseGameplayAbility>(AbilitySpec.Ability))
	{
		//DynamicAbilityTags은 런타임중에 Tag 추가 및 제거가 가능하다.
		AbilitySpec.DynamicAbilityTags.AddTag(BaseAbility->StartupInputTag);
		AbilitySpec.InputID = 1;
		return GiveAbility(AbilitySpec);
	}

	return FGameplayAbilitySpecHandle();
}

void UGoblinAbilitySystemComponent::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.IsValid() == false)
		return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			//nvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			if (AbilitySpec.IsActive() == false)
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}

}


void UGoblinAbilitySystemComponent::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (InputTag.IsValid() == false)
		return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UGoblinAbilitySystemComponent::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (InputTag.IsValid() == false)
		return;


	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive() == false)
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}


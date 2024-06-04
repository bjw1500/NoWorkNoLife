// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/BaseGameplayAbility.h"
#include "DamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GOBLIN_API UDamageGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:

	/*
		적용할 Damage Effect.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Goblin | Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/*
		Damage Type
		키 => 데미지 속성
		값 => 적용할 데미지
		지금은 기획이 없어서 복잡하게 다루지는 않을 건데.
		필요하면 원신처럼 속성간의 시너지까지 시도.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Goblin | Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;


	UFUNCTION(BlueprintCallable, Category = "Goblin | Damage")
	void CauseDamage(AActor* TargetActor);

};

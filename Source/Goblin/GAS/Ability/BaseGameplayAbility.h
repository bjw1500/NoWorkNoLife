// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GOBLIN_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:

	/*
		해당 어빌리티를 사용할 때 쓸 Input Tag
		단어 그대로 키 입력을 설정한다.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Goblin | Input")
	FGameplayTag StartupInputTag;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Managers/GameplayTagManager.h"
#include "GoblinInputConfig.generated.h"

/*
	InputAction과 GameplayTag를 연동하는 구조체.
	이걸 통해 EnhancedInputSystem과 결합하여 런타임중에 입력을 변경할 수 있게 한다.
*/
USTRUCT(BlueprintType)
struct FGoblinInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	 FGameplayTag InputTag = FGameplayTag();

};

UCLASS()
class GOBLIN_API UGoblinInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	/*GameplayTag를 건네받으면, AbilityInputActions에 맵핑된 Inpuat Action을 꺼내준다.*/
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGoblinInputAction> AbilityInputActions;
};

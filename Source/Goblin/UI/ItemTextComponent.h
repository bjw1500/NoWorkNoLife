// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ItemTextComponent.generated.h"

/**
 * 
 */
 class AItemBase;
 struct FItemData;

UCLASS()
class GOBLIN_API UItemTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:

	//함수 구현은 블루프린트에서.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetItemText(FItemData ItemData);
};

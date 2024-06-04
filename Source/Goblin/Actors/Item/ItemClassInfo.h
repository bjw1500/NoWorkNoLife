// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Interface/CombatInterface.h"
#include "Animations/GoblinAnimInstance.h"
#include "ItemClassInfo.generated.h"

/*
	Item
	DataAsset VS DataTable.
	어떤걸 쓸까.

*/

class UGameplayEffect;
class UGameplayAbility;
class UCurveTable;
class AItemBase;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	WEAPON,
	COUNSUMABLE,
	SaleItem,
	Mushroom,
	MapObject,
	None,
};


UENUM(BlueprintType)
enum class ERarity : uint8
{
	Normal,
	Magic,
	Rare,
	Unique,
	None,
};

USTRUCT(Atomic, BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERarity ItemRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EItemType> ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAnimationState AnimationType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> StartupAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTaggedMontage TaggedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemBase> Model;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ObjectScript;					//포탈 작동하려면 [E], 문 열기 [E] 이런 스크립트용 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ActionBarTime = 0;

	bool HasActionBarTime()
	{
		if (ActionBarTime > 0)
			return true;
		return false;
	}

	bool CanPickUp()
	{
		if (ItemType >= EItemType::MapObject)
			return false;
		return true;
	}
};

/**
 *
 */
UCLASS()
class GOBLIN_API UItemClassInfo : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Goblin")
	TMap<int, FItemData> WeaponItem;

	UPROPERTY(EditDefaultsOnly, Category = "Goblin")
	TMap<int, FItemData> ArmorItem;

	UPROPERTY(EditDefaultsOnly, Category = "Goblin")
	TMap<int, FItemData> Consumable;

};

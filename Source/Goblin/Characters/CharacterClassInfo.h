// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Actors/Item/ItemClassInfo.h"
#include "CharacterClassInfo.generated.h"
class UGameplayEffect;
class UGameplayAbility;
class UCurveTable;
class ABaseCharacter;



/*
	컨텐츠 제작 들어가면 분류 제대로 해주기.
*/
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	None,
	Mage,
	NoBrain,
	NoHeart,
	Thief,
	Slime,
	Mimic,
	NoHead,
	Player,

	//리셀 컴퍼니 
	NoLeft,
	NoBody,
	NoLowBody,
	Elysia,

	//
	Doppelganger,
	Centipede,
};


UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Human,
	Monster,
	None,
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	//해당 캐릭터의 Stat
	UPROPERTY(EditDefaultsOnly, Category = "Class Common")
	TSubclassOf<UGameplayEffect> MainStatAttributes;

	//해당 캐릭터의 스킬.
	UPROPERTY(EditDefaultsOnly, Category = "Class Common")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Class Common")
	ECharacterClass CharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Class Common")
	ECharacterType CharacterType;

	UPROPERTY(EditDefaultsOnly, Category = "Class Common")
	ERarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseCharacter> Model;
};


UCLASS()
class GOBLIN_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Goblin")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Goblin")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Goblin")
	TObjectPtr<UCurveTable> DamageCalculationCofficients;

	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable)
	TArray<FCharacterClassDefaultInfo>	GetClassDefaultInfoByRarity(ERarity Rariry);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "UI/GoblinUserWidget.h"
#include "UnrealStruct.generated.h"


/*
	나중에 구조체를 만들일이 생기면 여기다가 만들어주자.
*/

class ACharacter;
class AActor;
class UTexture2D;
class UBaseGameplayAbility;
class AItemBase;

USTRUCT(BlueprintType, Blueprintable)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGoblinUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D*	Image = nullptr;
};

USTRUCT(Atomic, BlueprintType)
struct FRewardData : public FTableRowBase
{
	GENERATED_BODY()

		//아이템 드랍 확률
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float ItemDropRate;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Id;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FText ItemName = FText();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Count;
};


UENUM()
enum EMonsterType
{
	Normal,
	Boss,
};

USTRUCT(Atomic, BlueprintType)
struct FMonsterData : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int32 Id;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText MonsterName = FText();

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EMonsterType> MonsterType;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<FRewardData> RewardDatas;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ACharacter> Model;
};


UENUM()
enum SkillType
{
	Dash,
	SKILL_1,
	SKILL_2,
	SKILL_3,
};

USTRUCT(Atomic, BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString Name;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<SkillType> SkillType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int32 CoolTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int32 Damage;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<UBaseGameplayAbility> SkillObject;
};
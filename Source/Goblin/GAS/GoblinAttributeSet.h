// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GoblinAttributeSet.generated.h"


/*
	AttributeSet의 변수들을  정리하는데 사용하는 매크로를 한 곳에 묶어주었다.
*/
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/*이해하기를 포기한 코드. 아무튼 대충 사용해보면 느낌은 알 수 있으니 그냥 쓰자.*/
DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;


USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};


UCLASS()
class GOBLIN_API UGoblinAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:

	UGoblinAttributeSet();

	/*Replicate 할 변수들을 등록하는 함수.*/
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//AttributeSet에 등록된 값들이 변경될 때마다 자동으로 호출되는 함수.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	//GameplayEffect가 적용될 때마다 호출되는 함수
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	//Data에서 우리가 사용하기 편하게 필요한 정보를 빼네 FEffectProperties 구조체 안에  담는다.
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;

	/*
		게임에 필요한 Atrribute들을 등록해주자.
		지금은 어떤 Atrribute들이 필요한지 정확히 알 수 없으니 체력과 스태미너만 넣어주었다.
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Goblin | Stat")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGoblinAttributeSet, Health);

		UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Goblin | Stat")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGoblinAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Goblin | Stat")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGoblinAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Goblin | Stat")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGoblinAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentMoveSpeed, Category = "Goblin | Stat")
	FGameplayAttributeData CurrentMoveSpeed;
	ATTRIBUTE_ACCESSORS(UGoblinAttributeSet, CurrentMoveSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseMoveSpeed, Category = "Goblin | Stat")
	FGameplayAttributeData BaseMoveSpeed;
	ATTRIBUTE_ACCESSORS(UGoblinAttributeSet, BaseMoveSpeed);

	/*
		몬스터가 사용할 Stat
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SearchRange, Category = "Goblin | MonsterStat")
	FGameplayAttributeData SearchRange;
	ATTRIBUTE_ACCESSORS(UGoblinAttributeSet, SearchRange);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HearingRange, Category = "Goblin | MonsterStat")
	FGameplayAttributeData HearingRange;
	ATTRIBUTE_ACCESSORS(UGoblinAttributeSet, HearingRange);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Goblin | MonsterStat")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UGoblinAttributeSet, AttackSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_OpenDoorSpeed, Category = "Goblin | MonsterStat")
	FGameplayAttributeData OpenDoorSpeed;
	ATTRIBUTE_ACCESSORS(UGoblinAttributeSet, OpenDoorSpeed);


	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;

		UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;

	UFUNCTION()
	void OnRep_CurrentMoveSpeed(const FGameplayAttributeData& OldCurrentMoveSpeed) const;

	UFUNCTION()
	void OnRep_BaseMoveSpeed(const FGameplayAttributeData& OldBaseMoveSpeed) const;

	UFUNCTION()
	void OnRep_SearchRange(const FGameplayAttributeData& OldSearchRange) const;

	UFUNCTION()
	void OnRep_HearingRange(const FGameplayAttributeData& OldHearingRange) const;

	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const;

	UFUNCTION()
	void OnRep_OpenDoorSpeed(const FGameplayAttributeData& OldOpenDoorSpeed) const;

	/*
		Meta Attributes
		저장되지 않고 일시적인 속성들.
		서버에서만 계산되는 수치들.
	*/

	UPROPERTY(BlueprintReadOnly, Category = "Goblin | Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UGoblinAttributeSet, IncomingDamage);

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;
};

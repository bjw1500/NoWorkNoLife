// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "CombatInterface.generated.h"


class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Montage  = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;

};





// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GOBLIN_API ICombatInterface
{
	GENERATED_BODY()


public:
	
	virtual int32		GetLevel();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector	GetWeaponSocketLocation(FGameplayTag GameplayTag);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void		UpdateFacingTarget(const FVector& Target);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	 void			SetCombatTarget(AActor* InCombatTarget);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	 AActor* GetCombatTarget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	 UAnimMontage* GetHitReactMontage();

	 UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	 UAnimMontage* GetStunReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	 UNiagaraSystem* GetBloodEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetSkillMontage();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	 FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontagedTag);

	 UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	 void SetLadder(AActor* Ladder);

	 UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	 AActor* GetLadder();

	 UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	 void GrabLadder();

	 UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	 UAnimMontage* GetClimbingLadderMontage();

	virtual void Die() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const; 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar() const;
};

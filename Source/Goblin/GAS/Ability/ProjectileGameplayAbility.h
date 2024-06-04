// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/DamageGameplayAbility.h"
#include "ProjectileGameplayAbility.generated.h"

class AGoblinProjectile;
class UGameplayEffect;

UCLASS()
class GOBLIN_API UProjectileGameplayAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()
public:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	
	UFUNCTION(BlueprintCallable, Category = "Goblin | Projectile")
	void SpawnProjectile(const FVector& ProjectileSpawnLocation, const FVector& ProjectileTargetLocation, bool HasTarget = true);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goblin | Projectile")
	TSubclassOf<AGoblinProjectile> ProjectileClass;
	
};

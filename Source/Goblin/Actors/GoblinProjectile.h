// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "GoblinProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UAudioComponent;

UCLASS()
class GOBLIN_API AGoblinProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoblinProjectile();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	void SpawnEffect();

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;


	/*
		Impact
	*/

	UPROPERTY(EditAnywhere, Category ="Goblin | Impact")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, Category = "Goblin | Impact")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Goblin | Impact")
	TObjectPtr<USoundBase> LoopingSound;
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Goblin | ObjectInfo")
	float LifeSpan = 15.f;

	UPROPERTY(EditDefaultsOnly, Category = "Goblin | ObjectInfo")
	float Speed = 550.f;

	bool bHit = false;

public:

	/*
		DamageEffectSpectHandle.
		타겟팅한 상대방에게 적용할 GameplayEffect의 Handle.
		투사체를 생성하는 GameAbility에서 생성 후 이곳에 넣어준다.
	*/
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
};

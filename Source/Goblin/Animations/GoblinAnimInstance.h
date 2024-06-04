// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GoblinAnimInstance.generated.h"

/**
 * 
 */

 class ABaseCharacter;

 UENUM(BlueprintType)
enum class EAnimationState : uint8
{
	None,
	EquipWeapon,
	HoldingOneHand,
	HoldingTwoHand,
	HoldingLight,
};

 UENUM(BlueprintType)
enum class ECharacterMoveState : uint8
{
	Idle,
	Run,
	Crouch,
};

UCLASS()
class GOBLIN_API UGoblinAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UGoblinAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:

	TObjectPtr<ABaseCharacter> Owner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goblin | Move")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goblin | Move")
	EAnimationState AnimationState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goblin | Move")
	ECharacterMoveState MoveState = ECharacterMoveState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goblin | Move")
	bool HasFlashlight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goblin | Move")
	float MoveForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goblin | Move")
	float MoveRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goblin | Move")
	float TargetLowerRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goblin | Move")
	float CurrentLowerRotation = 0.f;

	//플레이어가 바라보는 방향을 향해 머리를 숙이기 위한 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goblin | Move")
	float ControllerPitch = 0.f;

	//상대방을 향해 고개를 돌리기 위한 변수.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goblin | Move")
	float ControllerYaw = 0.f;
};

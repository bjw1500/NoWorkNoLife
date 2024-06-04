// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "AbilitySystemInterface.h"
#include "CharacterClassInfo.h"
#include "Interface/ObjectInterface.h"
#include "Animations/GoblinAnimInstance.h"
#include "BaseCharacter.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
struct FGameplayTag;
class AWeaponItem;
class AItemBase;
class UNiagaraSystem;
class UGoblinAnimInstance;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateHitResult, FHitResult, HitResult);




UCLASS()
class GOBLIN_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface, public IObjectInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
	virtual void InitAbilityActorInfo();									//설정된 정보대로 GameAbility를 초기화해준다. 
	virtual void InitDefaultAttributes() const;					//설정된 DefaultAttribute 정보대로 초기화해준다.
	void				ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float ActorLevel) const;



	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();							//캐릭터 죽음은 모든 클라이언트가 알아야 한다. 그래서 NetMulticast.
						
public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SnapToSurface();														//게임 중에 스폰할때 바닥에 붙게해주기 위한 함수.

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnBloodEffect();			
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RemoveDeadBodyBloodDecal();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> DeadBodyBloodDecal;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintCallable)
	UAttributeSet*											GetAttributeSet() const { return AttributeSet; }


	/*Combat*/
	FVector															GetWeaponSocketLocation_Implementation(FGameplayTag GameplayTag);
	virtual UAnimMontage*						GetHitReactMontage_Implementation() override;
	virtual UAnimMontage*						GetStunReactMontage_Implementation() override;
	virtual UNiagaraSystem*						GetBloodEffect_Implementation();
	virtual AActor*											GetAvatar_Implemantation();
	virtual TArray<FTaggedMontage>		GetAttackMontage_Implementation() override;
	virtual TArray<FTaggedMontage>		GetSkillMontage_Implementation() override;
	virtual FTaggedMontage						GetTaggedMontageByTag_Implementation(FGameplayTag& MontagedTag);
	virtual void													SetCombatTarget_Implementation(AActor* InCombatTarget);
	virtual AActor*											GetCombatTarget_Implementation();

	virtual void													Die() override;
	virtual bool													IsDead_Implementation() const;

	virtual void													Jump() override;

	/*사다리 타기 관련*/
	//GrabLadder 함수는 블루프린트에서 구현해주었다.
	virtual void													SetLadder_Implementation(AActor* Target);
	virtual AActor*											GetLadder_Implementation();
	virtual UAnimMontage*						GetClimbingLadderMontage_Implementation();
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Goblin | PlayerInterface")
	void TryChangeFlyingSpeed(float Value);

	/*IObjectInterface*/
	virtual void HighlightObject(AGoblinPlayerController* Player) override;
	virtual void UnHighlightObject(AGoblinPlayerController* Player) override;


	/*애니메이션 관련*/
	UFUNCTION(Server, Unreliable)
	void Server_SetControllerPitch(float Value);

	/*속도 변화*/
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_ChangeSpeed(float Value);

	UFUNCTION(Client, BlueprintCallable, Reliable)
	void Multicast_ChangedSpeed(float Value);

public:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Ladder)
	bool OnLadder = false;
	UFUNCTION()
	void OnRep_Ladder() const;


	/*
		Combat
	*/
	//UPROPERTY(EditAnywhere, BlueprintReadOnly ,Category = "Goblin | Combat")
	//TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Goblin | Combat")
	TObjectPtr<AItemBase> HeldObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Goblin | Combat")
	TObjectPtr<AItemBase> HeldAbilityItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Goblin | Combat")
	TObjectPtr<ABaseCharacter> HeldDeadBody;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Goblin | Combat")
	TObjectPtr<ABaseCharacter> DeadBodyOwner;

	UPROPERTY(EditAnywhere, Category = "Goblin | Combat")
	FName WeaponTipSocketName = FName("WeaponTipSocket");
	
	UPROPERTY(EditAnywhere, Category = "Goblin | Combat")
	FName RightHandSocketName = FName("RightHandSocket");

	UPROPERTY(EditAnywhere, Category = "Goblin | Combat")
	FName LeftHandSocketName = FName("LeftHandSocket");

	UPROPERTY(EditAnywhere, Category = "Goblin | Combat")
	FName TwoHandSocketName = FName("TwoHandSocket");

	UPROPERTY(EditAnywhere, Category = "Goblin | Combat")
	FName WeaponHandSocketName = FName("WeaponHandSocket");

	UPROPERTY(EditAnywhere, Category = "Goblin | Combat")
	FName FlashlightSocketName = FName("FlashlightSocket");

	UPROPERTY(EditAnywhere, Category = "Goblin | Combat")
	FName HoldingMushRoomRightHandSocketName = FName("HoldingMushRoomRightHandSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Goblin | Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Goblin | Combat")
	TObjectPtr<UAnimMontage> StunReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goblin | Combat")
	TObjectPtr<UAnimMontage> ClimbingLadderMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Goblin | Combat")
	TObjectPtr<UNiagaraSystem> BloodEffect;

	UPROPERTY(EditAnywhere, Category = "Goblin | Combat")
	TObjectPtr<AActor> Ladder;


	UPROPERTY(EditAnywhere, Category = "Goblin | Combat")
	TArray<FTaggedMontage>  AttackMontages;

	UPROPERTY(EditAnywhere, Category = "Goblin | Combat")
	TArray<FTaggedMontage>  SkillMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goblin | Combat")
	int32 Level = 1;

	UPROPERTY(BlueprintReadOnly, Category = "Goblin | Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Goblin | Combat")
	float BaseWalkSpeed = 250.f;

	/*애니메이션 관련 변수*/
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MoveForward	;			//양수면 앞. 음수면 뒤로 이동.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MoveRight;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Replicated)
	//bool HasFlashlight = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<UGoblinAnimInstance> AnimInstance;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Replicated)
	EAnimationState AnimationState;

	UPROPERTY(EditAnywhere, Replicated, Category = "Goblin | ObjectInterface")
	float ControllerPitch = 0.f;

	/*											*/	

	UPROPERTY(BlueprintReadOnly, Category = "Goblin | Combat")
	float LifeSpan = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goblin | Combat")
	ECharacterClass CharacterClass = ECharacterClass::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Goblin | Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated,Category = "Goblin | Combat")
	bool bDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated,Category = "Goblin | Combat")
	bool bStunned = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated,Category = "Goblin | Combat")
	bool bUnDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Goblin | Combat")
	bool CanMoveLadder = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = "Rep_CharacterMoveState", Category = "Goblin | Combat")
	ECharacterMoveState CharacterMoveState = ECharacterMoveState::Idle;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SetCharacterMoveState(ECharacterMoveState State);

	UFUNCTION()
	void Rep_CharacterMoveState();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Rep_CharacterMoveStateEvent();

	//False로 켜두면 추적시 재생되는 공포 사운드에 영향을 주지 않는다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated,Category = "Goblin | Combat")
	bool BeingChased = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goblin | Combat")
	float FallingDamage = 0.f;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FDelegateHitResult DelegaeHitResult;

	UFUNCTION(BlueprintCallable)
	void DelegateHitResultBroadCast(FHitResult Result);
};

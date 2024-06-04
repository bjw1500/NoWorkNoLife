// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"

#include "Goblin.h"
#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"
#include "GAS/GoblinAbilitySystemComponent.h"
#include "GAS/GoblinAttributeSet.h"
#include "Managers/GameplayTagManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actors/Item/ItemBase.h"
#include "Actors/Item/WeaponItem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Animations/GoblinAnimInstance.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	/*무기를 장착할 경우 들어줄 위치를 설정해준다.*/
	//Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	//Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	//Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*카메라 충돌 문제 해결*/
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	//투사체에 대한 충돌 설정
	//GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
	{
		AnimInstance = Cast<UGoblinAnimInstance>(AnimInstance);
	}
	SnapToSurface();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, OnLadder);
	DOREPLIFETIME(ABaseCharacter, HeldObject);
	DOREPLIFETIME(ABaseCharacter, HeldDeadBody);
	DOREPLIFETIME(ABaseCharacter, DeadBodyOwner);
	DOREPLIFETIME(ABaseCharacter, bUnDead);
	DOREPLIFETIME(ABaseCharacter, BeingChased);
	DOREPLIFETIME(ABaseCharacter, CharacterMoveState);
	DOREPLIFETIME(ABaseCharacter, ControllerPitch);
	DOREPLIFETIME(ABaseCharacter, AnimationState);
	DOREPLIFETIME(ABaseCharacter, bDead);
	DOREPLIFETIME(ABaseCharacter, bStunned);
	DOREPLIFETIME(ABaseCharacter, CombatTarget);
	DOREPLIFETIME(ABaseCharacter, HeldAbilityItem);
	//DOREPLIFETIME(ABaseCharacter, HasFlashlight);
}

void ABaseCharacter::InitAbilityActorInfo()
{
}

void ABaseCharacter::InitDefaultAttributes() const
{
	UGoblinBlueprintFunctionLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void ABaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float ActorLevel) const
{
	check(IsValid(GameplayEffect));
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect, ActorLevel, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ABaseCharacter::Die()
{
	if (HeldObject != nullptr)
	{
		//Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	
	}
	MulticastHandleDeath();
	bDead = true;

	if(HasAuthority())
		SpawnBloodEffect();
}

void ABaseCharacter::MulticastHandleDeath_Implementation()
{
	//if (Weapon != nullptr)
	//{
	//	Weapon->SetSimulatePhysics(true);
	//	Weapon->SetEnableGravity(true);
	//	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	//};


	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->SetCollisionObjectType(ECC_Item);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


bool ABaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

void ABaseCharacter::Jump()
{
	//사다리에 붙은 상태로 점프를 누르면 사다리 올라가기.
	//if (IsValid(Ladder))
	//{
	//	ICombatInterface::Execute_GrabLadder(this);
	//}
	//else
	//{
	//	Super::Jump();
	//}
	Super::Jump();

}

AActor* ABaseCharacter::GetAvatar_Implemantation()
{
	return this;
}

TArray<FTaggedMontage> ABaseCharacter::GetAttackMontage_Implementation()
{
	return AttackMontages;
}

TArray<FTaggedMontage> ABaseCharacter::GetSkillMontage_Implementation()
{
	return SkillMontages;
}

FTaggedMontage ABaseCharacter::GetTaggedMontageByTag_Implementation(FGameplayTag& MontagedTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontagedTag)
		{
			return TaggedMontage;
		}
	}

	return FTaggedMontage();
}



UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FVector ABaseCharacter::GetWeaponSocketLocation_Implementation(FGameplayTag GameplayTag)
{

	const FGameplayTagManager& TagManager = FGameplayTagManager::Get(); 
	
	if (GameplayTag.MatchesTagExact(TagManager.CombatSocket_Weapon))
	{

		/*
			HeldObject => 플레이어에게서 무기 소켓 위치를 가져올 때.
			Weapon		=> 몬스터나 모험가들에게서 무기 소켓 위치를 가져올 때.

			플레이어는 아이템 오브젝트 형태로 무기를 들고 있을 거라, HeldObejct에서 위치를 가져오고,
			몬스터나 모험가들은 모델링만 가지고 있기에 Weapon에서 위치를 가져온다.
		*/

		if(HeldObject)
		{ 
			AWeaponItem* WeaponItem = Cast<AWeaponItem>(HeldObject);
			if(WeaponItem == nullptr)
				return FVector::Zero();
			return WeaponItem->GetMesh()->GetSocketLocation(WeaponTipSocketName);
		}
	}
	
	if (GameplayTag.MatchesTagExact(TagManager.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}

	if (GameplayTag.MatchesTagExact(TagManager.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}

	return FVector::Zero();
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

UAnimMontage* ABaseCharacter::GetStunReactMontage_Implementation()
{
	return StunReactMontage;
}

UNiagaraSystem* ABaseCharacter::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

void ABaseCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ABaseCharacter::GetCombatTarget_Implementation()
{
	return CombatTarget;
}

void ABaseCharacter::SetLadder_Implementation(AActor* Target)
{
	if(OnLadder == false)
		Ladder = Target;
}

AActor* ABaseCharacter::GetLadder_Implementation()
{
	return Ladder;
}

UAnimMontage* ABaseCharacter::GetClimbingLadderMontage_Implementation()
{
	return ClimbingLadderMontage;
}

void ABaseCharacter::HighlightObject(AGoblinPlayerController* Player)
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ABaseCharacter::UnHighlightObject(AGoblinPlayerController* Player)
{
	GetMesh()->SetRenderCustomDepth(false);
}

void ABaseCharacter::Server_ChangeSpeed_Implementation(float Value)
{
	GetCharacterMovement()->MaxWalkSpeed = Value;
	Multicast_ChangedSpeed(Value);
}

void ABaseCharacter::Multicast_ChangedSpeed_Implementation(float Value)
{
	GetCharacterMovement()->MaxWalkSpeed = Value;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Client Speed Change %f"), GetCharacterMovement()->MaxWalkSpeed));
}

void ABaseCharacter::Server_SetControllerPitch_Implementation(float Value)
{
	ControllerPitch = Value;
}

void ABaseCharacter::TryChangeFlyingSpeed_Implementation(float Value)
{
	GetCharacterMovement()->MaxFlySpeed = Value;
}


void ABaseCharacter::OnRep_Ladder() const
{
}

void ABaseCharacter::Server_SetCharacterMoveState_Implementation(ECharacterMoveState State)
{
	CharacterMoveState = State;
	Rep_CharacterMoveStateEvent();
}

void ABaseCharacter::Rep_CharacterMoveState()
{
	Rep_CharacterMoveStateEvent();
}

void ABaseCharacter::DelegateHitResultBroadCast(FHitResult Result)
{
	DelegaeHitResult.Broadcast(Result);
}

void ABaseCharacter::SnapToSurface()
{
	FVector StartLoc = GetActorLocation();
	FVector EndLoc = GetActorLocation() - FVector(0, 0, 1000.f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
	TEnumAsByte<EObjectTypeQuery> MapObject = UEngineTypes::ConvertToObjectType(ECC_MapObject);
	ObjectTypes.Add(WorldStatic);
	ObjectTypes.Add(WorldDynamic);
	ObjectTypes.Add(MapObject);

	TArray<AActor*> IgnoreActors; // 무시할 액터들.

	FHitResult HitResult; // 히트 결과 값 받을 변수.

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc,
		ObjectTypes,
		false,
		IgnoreActors, // 무시할 것이 없다고해도 null을 넣을 수 없다.
		EDrawDebugTrace::None,
		HitResult,
		true
		// 여기 밑에 3개는 기본 값으로 제공됨. 바꾸려면 적으면 됨.
		//, FLinearColor::Red
		//, FLinearColor::Green
		//, 5.0f
	);

	if (Result == true)
	{
		// FVector ImpactPoint = HitResult.ImpactPoint;
		// HitResult에서 필요한 값들을 사용하면 됨.
		FVector Normal = HitResult.ImpactNormal * 50;
		SetActorLocation(HitResult.ImpactPoint + Normal);
	}

}

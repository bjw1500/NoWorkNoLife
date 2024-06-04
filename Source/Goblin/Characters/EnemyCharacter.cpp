// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "GAS/GoblinAbilitySystemComponent.h"
#include "GAS/GoblinAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"
#include "AI/GoblinAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Managers/GameplayTagManager.h"
#include "Actors/Item/ItemBase.h"
#include "Actors/Item/WeaponItem.h"
#include "Player/GoblinPlayerController.h"
#include "Characters/PlayerCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
	//Collision 설정은 전부 Custom으로 변경
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UGoblinAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UGoblinAttributeSet>("AttributeSet");
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	//GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	UGoblinBlueprintFunctionLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);

	UGoblinAttributeSet* AS = Cast<UGoblinAttributeSet>(AttributeSet);
	if (IsValid(AS) == true)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetSearchRangeAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnSearchRangeChanged.Broadcast(Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHearingRangeAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHearingRangeChanged.Broadcast(Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetAttackSpeedAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnAttackSpeedChanged.Broadcast(Data.NewValue);
			});

		if (GoblinAIController)
		{
			GoblinAIController->GetBlackboardComponent()->SetValueAsFloat(FName("SearchRange"), AS->GetSearchRange());
			GoblinAIController->GetBlackboardComponent()->SetValueAsFloat(FName("HearingRange"), AS->GetHearingRange());
			GoblinAIController->GetBlackboardComponent()->SetValueAsFloat(FName("AttackSpeed"), AS->GetAttackSpeed());
		}
	}

	//GameplayTag가 추가 및 제거될 때마다 실행되는 델리게이트에 등록.
	//HitReact Effect가 적용될 때마다 호출될 것이다.
	AbilitySystemComponent->RegisterGameplayTagEvent(
		FGameplayTagManager::Get().Effects_HitReact,
		EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemyCharacter::HitReactTagChanged);
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//서버에서만 실행한다.
	if (HasAuthority() == false)
		return;

	GoblinAIController = Cast<AGoblinAIController>(NewController);

	GoblinAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->GetBlackboardAsset());
	GoblinAIController->RunBehaviorTree(BehaviorTree);
	GoblinAIController->RunBehaviorTree(BehaviorTree);
	GoblinAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	//GoblinAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);

	UGoblinAttributeSet* GAS = Cast<UGoblinAttributeSet>(AttributeSet);
	if(GAS)
	{ 
		OnSearchRangeChanged.AddLambda(
		[this](float NewValue){
				GoblinAIController->GetBlackboardComponent()->SetValueAsFloat(FName("SearchRange"), NewValue);
		});
		GoblinAIController->GetBlackboardComponent()->SetValueAsFloat(FName("SearchRange"), GAS->GetSearchRange());

		OnHearingRangeChanged.AddLambda(
			[this](float NewValue) {
				GoblinAIController->GetBlackboardComponent()->SetValueAsFloat(FName("HearingRange"), NewValue);
			});
		GoblinAIController->GetBlackboardComponent()->SetValueAsFloat(FName("HearingRange"), GAS->GetHearingRange());

		OnAttackSpeedChanged.AddLambda(
			[this](float NewValue) {
				GoblinAIController->GetBlackboardComponent()->SetValueAsFloat(FName("AttackSpeed"), NewValue);
			});
		GoblinAIController->GetBlackboardComponent()->SetValueAsFloat(FName("AttackSpeed"), GAS->GetAttackSpeed());
	}


	//HeldObject = GetWorld()->SpawnActor<AWeaponItem>(MonsterWeapon);
	//Server_PickUpItem(HeldObject);
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UGoblinAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitDefaultAttributes();
}

void AEnemyCharacter::InitDefaultAttributes() const
{
	UGoblinBlueprintFunctionLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AEnemyCharacter::Server_PickUpItem_Implementation(AItemBase* Item)
{
	//아이템을 주운다.
	//그런데 Rep 옵션을 어떻게 켜줘야할까?


	if (Item == nullptr)
		return;

	//손에 시체를 쥐고 있다면,
	if (HeldDeadBody != nullptr)
		return;

	//아이템을 쥐고 있다면,
	if(HeldObject != nullptr)
		return;

	if (Item->ItemOwner)
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(Item->ItemOwner))
		{
			if (AGoblinPlayerController* PC = Cast<AGoblinPlayerController>(Player->GetController()))
			{
				PC->DropItem();
			}
		}
	}

	//MulticastPickUpItem(Item);
	Item->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Item->GetMesh()->SetSimulatePhysics(false);
	Item->GetMesh()->SetEnableGravity(false);
	HeldObject = Item;
	Item->ItemOwner = this;
	Item->GetMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandSocketName);
	//Item->ItemAbilitySpecHandle = Cast<UGoblinAbilitySystemComponent>(AbilitySystemComponent)->AddCharacterAbility(Item->ItemData.StartupAbility);
}

void AEnemyCharacter::TryPickUpDeadBody_Implementation(ABaseCharacter* DeadBody, bool IsSwapCheck)
{
	//아이템을 주운다.
	//그런데 Rep 옵션을 어떻게 켜줘야할까?

	if (DeadBody == nullptr)
		return;

	if (DeadBody->DeadBodyOwner != nullptr)
		return;

	//손에 시체를 쥐고 있다면,
	if (HeldDeadBody != nullptr)
		return;

	//현재 손에 들고 있는 무기나 아이템 있다면? 시체를 줍지 못한다.
	if (HeldObject != nullptr)
	{
		return;
	}
	else
	{
		DeadBody->DeadBodyOwner = this;
		HeldDeadBody = DeadBody;
	}
	MulticastPickUpDeadBody(DeadBody);
	AnimationState = EAnimationState::HoldingTwoHand;
}

void AEnemyCharacter::MulticastPickUpDeadBody_Implementation(ABaseCharacter* DeadBody, bool IsSwapCheck)
{
	DeadBody->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DeadBody->GetMesh()->SetSimulatePhysics(false);
	DeadBody->GetMesh()->SetEnableGravity(false);

	/*애니메이션 모션 변화*/
	DeadBody->GetMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandSocketName);

	for (AActor* Decal : DeadBody->DeadBodyBloodDecal)
	{
		Decal->AttachToComponent(DeadBody->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}


void AEnemyCharacter::MulticastPickUpItem_Implementation(AItemBase* Item)
{
	if(Item == nullptr)
		return;

 	Item->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Item->GetMesh()->SetSimulatePhysics(false);
	Item->GetMesh()->SetEnableGravity(false);
	HeldObject = Item;
}


void AEnemyCharacter::Server_DropItem_Implementation()
{

	//시체를 버릴때.
	if (HeldDeadBody)
	{
		MulticastDropItem();
		return;
	}


	if (HeldObject == nullptr)
		return;

	HeldObject->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	AbilitySystemComponent->ClearAbility(HeldObject->ItemAbilitySpecHandle);
	MulticastDropItem();
}

void AEnemyCharacter::MulticastDropItem_Implementation()
{

	if(HeldDeadBody)
	{ 
		HeldDeadBody->DeadBodyOwner = nullptr;
		HeldDeadBody->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		HeldDeadBody->GetMesh()->SetSimulatePhysics(true);
		HeldDeadBody->GetMesh()->SetEnableGravity(true);
		HeldDeadBody->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		HeldDeadBody = nullptr;
		/*애니메이션 모션 변화*/
		AnimationState = EAnimationState::None;
		return;
	}


	if(HeldObject == nullptr)
		return;
	HeldObject->GetMesh()->SetSimulatePhysics(true);
	HeldObject->GetMesh()->SetEnableGravity(true);
	HeldObject->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HeldObject = nullptr;
}


void AEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;

	//서버 전용 코드
	if(GoblinAIController)
	{ 
		//AI 컨트롤러는 서버상에서만 존재. 그렇기에 Client에서는 실행되면 안되는 코드
		GoblinAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

int32 AEnemyCharacter::GetLevel()
{
	return Level;
}

void AEnemyCharacter::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
	Server_DropItem();
	GoblinAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);

	if (IsValid(CombatTarget) == true)
	{
		APlayerCharacter* PC = Cast<APlayerCharacter>(CombatTarget);
		if(PC)
			GiveUpChase(PC);
	}
}

void AEnemyCharacter::HighlightObject(AGoblinPlayerController* Player)
{
	Server_HighlightObject(Player);
}

void AEnemyCharacter::UnHighlightObject(AGoblinPlayerController* Player)
{
	Server_UnHighlightObject(Player);
}

void AEnemyCharacter::Server_HighlightObject_Implementation(AGoblinPlayerController* Player)
{
	if (HasAuthority())
	{
		GoblinAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDetected"), true);
	}
}

void AEnemyCharacter::Server_UnHighlightObject_Implementation(AGoblinPlayerController* Player)
{
	if (HasAuthority())
	{
		GoblinAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDetected"), false);
	}
}

void AEnemyCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	//미완성 코드. 나중에 지울 수도 있음
	if(CombatTarget != InCombatTarget)
	{
			if(APlayerCharacter* LastPlayer = Cast<APlayerCharacter>(CombatTarget))
				GiveUpChase(LastPlayer);

			if (APlayerCharacter* Player = Cast<APlayerCharacter>(InCombatTarget))
				IsChasing(Player);
	}

	GoblinAIController->GetBlackboardComponent()->SetValueAsObject(FName("TargetToFollow"), InCombatTarget);
	CombatTarget = InCombatTarget;
}
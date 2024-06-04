// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/GoblinPlayerState.h"
#include "Player/GoblinPlayerController.h"
#include "GAS/GoblinAbilitySystemComponent.h"
#include "UI/GoblinHud.h"
#include "Actors/Item/ItemBase.h"
#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"
#include "Inventory/GoblinInventoryComponent.h"
#include "Interface/ObjectInterface.h"
#include "Camera/CameraComponent.h"
#include "Goblin.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Managers/GameplayTagManager.h"
#include "GameMode/GoblinGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

APlayerCharacter::APlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;


	//SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	//SpringArm->SetupAttachment(RootComponent);
	//SpringArm->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	//SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	//// Create a follow camera
	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	////FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->SetupAttachment(GetMesh());
	//FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	InventoryComponent = CreateDefaultSubobject< UGoblinInventoryComponent>(TEXT("Inventory Component"));
	InventoryComponent->SetIsReplicated(true);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	/*
		서버쪽에서 작동하는 코드.
	*/
	InitAbilityActorInfo();
	SetinventoryWidget();
	UGoblinBlueprintFunctionLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, ChasingMonsterCount);
}

void APlayerCharacter::OnRep_PlayerState()
{
	/*
	클라이언트를 위한 코드
	*/
	InitAbilityActorInfo();

	SetinventoryWidget();
}

void APlayerCharacter::TryPickUpItem_Implementation(AItemBase* Item, bool IsSwapCheck)
{
	if (Item == nullptr)
		return;

	if (Item->ItemOwner != nullptr)
		return;

	//손에 시체를 쥐고 있다면,`
	if(HeldDeadBody != nullptr)
		return;

		//여기서부터는 손전등을 제외한 기타 아이템
		//현재 손에 들고 있는 무기나 아이템 있다면?
	if (HeldObject != nullptr)
	{
		//TObjectPtr<AItemBase> TempItem = HeldObject;
		//TryDropItem();

		//TempItem->Destroy();
		//HeldObject = Item;
		//Item->ItemOwner = this;

		if (!IsSwapCheck)
		{
			InventoryComponent->Server_AddInventoryArray(Item->GetItem(), true);
		}

		if (Item)
		{ 
			Item->SetLifeSpan(0.1f);
		}
		return;
	}
	//비어 있으면 그냥 들어준다.
	else
	{
		Item->ItemOwner = this;

		HeldObject = Item;

		if (!IsSwapCheck)
		{
			InventoryComponent->Server_AddInventoryArray(Item->GetItem());
		}
	}


	////////////////////////////////////////////
	EquipItem(Item);
	Item->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Item->GetMesh()->SetSimulatePhysics(false);
	Item->GetMesh()->SetEnableGravity(false);

	/*애니메이션 모션 변화*/
	if (Item->ItemData.AnimationType == EAnimationState::EquipWeapon)
	{

		Item->GetMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponHandSocketName);
		AnimationState = EAnimationState::EquipWeapon;
	}
	else if (Item->ItemData.AnimationType == EAnimationState::HoldingLight)
	{
		
		Item->GetMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HoldingMushRoomRightHandSocketName);
		AnimationState = EAnimationState::HoldingLight;
	}
	else if (Item->ItemData.AnimationType == EAnimationState::HoldingOneHand)
	{
		Item->GetMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandSocketName);
		AnimationState = EAnimationState::HoldingOneHand;
	}
	else if (Item->ItemData.AnimationType == EAnimationState::HoldingTwoHand)
	{
		Item->GetMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TwoHandSocketName);
		AnimationState = EAnimationState::HoldingTwoHand;
	}
	Item->ItemAbilitySpecHandle = Cast<UGoblinAbilitySystemComponent>(AbilitySystemComponent)->AddCharacterAbility(Item->ItemData.StartupAbility);

}

void APlayerCharacter::TryPickUpDeadBody_Implementation(ABaseCharacter* DeadBody, bool IsSwapCheck)
{
	//아이템을 주운다.
	//그런데 Rep 옵션을 어떻게 켜줘야할까?

	if (DeadBody == nullptr)
		return;

	if (DeadBody->DeadBodyOwner != nullptr)
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
	Multicast_PickUpDeadBody(DeadBody);
	AnimationState = EAnimationState::HoldingTwoHand;
}

void APlayerCharacter::Multicast_PickUpDeadBody_Implementation(ABaseCharacter* DeadBody, bool IsSwapCheck)
{

	//DeadBody->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//DeadBody->GetCapsuleComponent()->SetSimulatePhysics(false);
	//DeadBody->GetCapsuleComponent()->SetEnableGravity(false);

	DeadBody->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DeadBody->GetMesh()->SetSimulatePhysics(false);
	DeadBody->GetMesh()->SetEnableGravity(false);

	/*애니메이션 모션 변화*/

	DeadBody->GetMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HeldDeadBodySocket"));
	//DeadBody->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HeldDeadBodySocket"));
	//DeadBody->MoveTo(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HeldDeadBodySocket"));
	

	for (AActor* Decal : DeadBody->DeadBodyBloodDecal)
	{
		Decal->AttachToComponent(DeadBody->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}


void APlayerCharacter::TryDropItem_Implementation()
{
	if(HasAuthority() == false)
		return;

	//시체를 버릴때.
	if (HeldDeadBody)
	{
		Multicast_DropDeadBody();
		return;
	}

	if (HeldObject == nullptr)
		return;

	HeldObject->ItemOwner = nullptr;
	DropItem(HeldObject);
	HeldObject->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	AbilitySystemComponent->ClearAbility(HeldObject->ItemAbilitySpecHandle);

	HeldObject->GetMesh()->SetSimulatePhysics(false);
	HeldObject->GetMesh()->SetEnableGravity(true);
	HeldObject->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HeldObject->SnapToGround();
	HeldObject = nullptr;

	/*애니메이션 모션 변화*/
	AnimationState = EAnimationState::None;
}

void APlayerCharacter::Die()
{
	//SetLifeSpan(LifeSpan);
	Super::Die();
	TryDropItem();
	DieEvent();

	AGoblinGameModeBase* GM = Cast<AGoblinGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->AddDeadPlayer(this->GetController());
	}

}


void APlayerCharacter::EquipItem_Implementation(AItemBase* Item)
{
	check(Item);

	Item->SetWidget();
}

int32 APlayerCharacter::GetLevel()
{
	AGoblinPlayerState* PS = GetPlayerState<AGoblinPlayerState>();

	return PS->GetPlayerLevel();
}

void APlayerCharacter::DieRightNow()
{
	Die();
}

void APlayerCharacter::InitAbilityActorInfo()
{
	AGoblinPlayerState* PS = GetPlayerState<AGoblinPlayerState>();
	if (IsValid(PS) == false)
		return;

	/*
		AbilitySystemComponent의 InitAbilityActorInfo 함수를 통해
		이후 AbilitySystemComponent의 실질적인 주인과 아바타를 설정해준다.
	*/
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	AbilitySystemComponent = PS->GetAbilitySystemComponent();
	AttributeSet = PS->GetAttributeSet();
	Cast<UGoblinAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();


	/*InGameUI 초기화*/
	AGoblinPlayerController* PC = Cast<AGoblinPlayerController>(GetController());
	if (IsValid(PC) == true)
	{
		AGoblinHUD* HUD = Cast<AGoblinHUD>(PC->GetHUD());
		if (IsValid(HUD) == true)
		{
			HUD->Init(PC, PS, AbilitySystemComponent, AttributeSet);
		}
	}

	/*DataAsset에 저장되어 있는 정보대로 캐릭어 Stat 초기화*/
	InitDefaultAttributes();
}

void APlayerCharacter::DropItem_Implementation(AItemBase* Item)
{
	Item->HideWidget();
}

void APlayerCharacter::Multicast_DropDeadBody_Implementation()
{
	HeldDeadBody->DeadBodyOwner = nullptr;
	HeldDeadBody->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	HeldDeadBody->GetMesh()->SetSimulatePhysics(true);
	HeldDeadBody->GetMesh()->SetEnableGravity(true);
	HeldDeadBody->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HeldDeadBody = nullptr;
	/*애니메이션 모션 변화*/
	AnimationState = EAnimationState::None;
}

void APlayerCharacter::TryUseItem_Implementation()
{
	//아이템을 버릴 때.
	if (HeldObject == nullptr)
		return;

	HeldObject->ItemOwner = nullptr;
	DropItem(HeldObject);
	GetInventoryComponent()->Server_RemoveItemDataToDrop();
	HeldObject->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	AbilitySystemComponent->ClearAbility(HeldObject->ItemAbilitySpecHandle);
	HeldObject->Destroy();
	HeldObject = nullptr;


	/*애니메이션 모션 변화*/
	AnimationState = EAnimationState::None;
}

void APlayerCharacter::TryPutInItem_Implementation()
{
	if(IsValid(HeldObject) == false)
		return;

	HeldObject->ItemOwner = nullptr;
	AbilitySystemComponent->ClearAbility(HeldObject->ItemAbilitySpecHandle);
	HeldObject->Destroy();
	HeldObject = nullptr;
	AnimationState = EAnimationState::None;
}

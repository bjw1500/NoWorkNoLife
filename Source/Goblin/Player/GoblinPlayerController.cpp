// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Input/GoblinEnhancedInputComponent.h"
#include "Input/GoblinInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Gameframework/Character.h"
#include "Managers/GameplayTagManager.h"
#include "GAS/GoblinAbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actors/Item/ItemBase.h"
#include "Goblin.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/BaseCharacter.h"
#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/GoblinHUD.h"
#include "Interface/ObjectInterface.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "Characters/Inventory/GoblinInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/GoblinGameStateBase.h"
#include "Managers/GameManager.h"


AGoblinPlayerController::AGoblinPlayerController()
{
}

void AGoblinPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(PlayerInputContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		//뒤에 숫자는 입력 우선 순위.
		Subsystem->AddMappingContext(PlayerInputContext, 0);
	}

	GameManager = Cast<UGameManager>(GetGameInstance());
}

void AGoblinPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UGoblinEnhancedInputComponent* GoblinInputComponent = CastChecked<UGoblinEnhancedInputComponent>(InputComponent);
	if (GoblinInputComponent)
	{
		GoblinInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGoblinPlayerController::Move);
		GoblinInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGoblinPlayerController::Look);
		//GoblinInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AGoblinPlayerController::ShiftPressed);
		//GoblinInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AGoblinPlayerController::ShiftReleased);
		GoblinInputComponent->BindAction(PickUpAction, ETriggerEvent::Completed, this, &AGoblinPlayerController::PickUpItem);
		GoblinInputComponent->BindAction(DropAction, ETriggerEvent::Completed, this, &AGoblinPlayerController::DropItem);
		GoblinInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AGoblinPlayerController::InteractItem);
		GoblinInputComponent->BindAction(SpaceAction, ETriggerEvent::Triggered, this, &AGoblinPlayerController::Jump);
		/*
			InputConfig DataAsset을 이용해서 액션 바인딩을 해준다.
				마우스 오른쪽 클릭 액션
				마우스 왼쪽 클릭 액션.
				1,2,3,4 버튼 액션.
		*/
		GoblinInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}

void AGoblinPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AGoblinPlayerController, DetectedTime, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(AGoblinPlayerController, WatchDuration, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(AGoblinPlayerController, EscapeSuccess, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(AGoblinPlayerController, PlayerCharacterState, COND_SimulatedOnly);
}

void AGoblinPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);



	//FindNearEnemy();
	//FindNearObject();
}

void AGoblinPlayerController::PlayerTick(float DeltaTime)
{
	/*
	플레이어 틱(Player Tick)
 
		플레이어 컨트롤러는 액터 클래스에서 상속받는 Tick() 함수 외에 PlayerTick() 이라는 별도의 틱 함수를 하나 더 가진다.
		일반 Tick() 함수는 어디서나 작동하는 반면에,
		PlayerTick() 함수는 Player Controller에 PlayerInput 객체가 있는 경우에만 호출된다.
		따라서 로컬로 제어되는 Player Controller에서만 플레이어 틱이 호출된다.
		이 말인 즉슨, 만약 멀티플레이 게임이라면 자기 자신의 플레이어 컨트롤러에서만 플레이어 틱이 호출된다는 것이다.
	*/
	Super::PlayerTick(DeltaTime);
	if (APlayerCharacter* PlayerPawn = Cast<APlayerCharacter>(GetPawn()))
		PlayerPawn->Server_SetControllerPitch(GetControlRotation().Pitch);
}

void AGoblinPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

UGoblinAbilitySystemComponent* AGoblinPlayerController::GetASC()
{
	if (GoblinAbilitySystemComponent == nullptr)
	{
		GoblinAbilitySystemComponent = Cast<UGoblinAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}

	return GoblinAbilitySystemComponent;
}

UGoblinInventoryComponent* AGoblinPlayerController::GetInventoryComponent()
{
	APlayerCharacter* PlayerPawn = Cast<APlayerCharacter>(GetPawn());
	if (PlayerPawn == nullptr)
		return nullptr;

	return PlayerPawn->GetInventoryComponent();
}

void AGoblinPlayerController::FindNearObject()
{
	APlayerCharacter* PlayerPawn = Cast<APlayerCharacter>(GetPawn());
	if (PlayerPawn == nullptr)
		return;

	//APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	//FVector camLocation = camManager->GetCameraLocation();
	//FVector camForward = camManager->GetCameraRotation().Vector();

	APlayerCameraManager* camManager = PlayerCameraManager;
	if(IsValid(camManager) == false)
		return;

	FVector camLocation = camManager->GetCameraLocation();
	FVector camForward = camManager->GetCameraRotation().Vector();
	FVector StartLoc = camLocation;
	FVector EndLoc = camLocation + camForward * 200;

	/*
		나중에 채널 추가해서 Item 추가해주기
	*/
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.

	//TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	//TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
	//ObjectTypes.Add(WorldStatic);
	//ObjectTypes.Add(WorldDynamic);
	TEnumAsByte<EObjectTypeQuery> Item = UEngineTypes::ConvertToObjectType(ECC_Item);
	TEnumAsByte<EObjectTypeQuery> Enemy = UEngineTypes::ConvertToObjectType(ECC_Enemy);
	TEnumAsByte<EObjectTypeQuery> MapObject = UEngineTypes::ConvertToObjectType(ECC_MapObject);
	ObjectTypes.Add(Item);
	ObjectTypes.Add(MapObject);

	TArray<AActor*> IgnoreActors; // 무시할 액터들.
	if(PlayerPawn->HeldObject != nullptr)
		IgnoreActors.Add(PlayerPawn->HeldObject);

	FHitResult HitResult; // 히트 결과 값 받을 변수.

	bool Result = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc,
		20.f,
		//FVector(30.f, 30.f, 100.f),
		//camLocation.Rotation(),
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
		IObjectInterface* FindObject = Cast<IObjectInterface>(HitResult.GetActor());
		if (FindObject == nullptr)
			return;

		LastObject = ThisObject;
		//ThisObject = Cast<AItemBase>(HitResult.GetActor());
		ThisObject = HitResult.GetActor();


		if(this != UGameplayStatics::GetPlayerController(this, 0))
			return;

		if (LastObject != ThisObject)
		{
			if(IsValid(LastObject) && LastObject->IsActorBeingDestroyed() == false)
			{ 
				if (IObjectInterface* LastObjectInterface = Cast<IObjectInterface>(LastObject))
				{
					LastObjectInterface->UnHighlightObject(this);
					OnStopActionBar.Broadcast();
					CloseScript();
				}
			}

			if(IsValid(ThisObject) && ThisObject->IsActorBeingDestroyed() == false)
			{ 

				if (ThisObject->GetClass()->ImplementsInterface(UObjectInterface::StaticClass()))
				{
					if (IObjectInterface* ThisObjectInterface = Cast<IObjectInterface>(ThisObject))
					{

						ThisObjectInterface->HighlightObject(this);
						if (AItemBase* ThisItem = Cast<AItemBase>(ThisObjectInterface))
							ShowScript(ThisItem);
					}
				}
			}
		}
	}
	else {

		if(ThisObject == nullptr)
			return;

		//플레이어의 시선이 닿지 않는다면 아이템의 하이라이트 효과를 꺼주자.
		if (ThisObject && IsValid(ThisObject) && ThisObject->IsActorBeingDestroyed() == false)
		{

			if (ThisObject->Implements<UObjectInterface>())
			{

				if (IObjectInterface* ThisObjectInterface = Cast<IObjectInterface>(ThisObject))
				{
					ThisObjectInterface->UnHighlightObject(this);
				}
			}
			OnStopActionBar.Broadcast();
			CloseScript();
			ThisObject = nullptr;

		}
	}
}

void AGoblinPlayerController::FindNearEnemy()
{
	APlayerCharacter* PlayerPawn = Cast<APlayerCharacter>(GetPawn());
	if (PlayerPawn == nullptr)
		return;

	//APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	//FVector camLocation = camManager->GetCameraLocation();
	//FVector camForward = camManager->GetCameraRotation().Vector();

	APlayerCameraManager* camManager = PlayerCameraManager;
	if (IsValid(camManager) == false)
		return;

	FVector camLocation = camManager->GetCameraLocation();
	FVector camForward = camManager->GetCameraRotation().Vector();
	FVector StartLoc = camLocation;
	FVector EndLoc = camLocation + camForward * 1000;

	/*
		나중에 채널 추가해서 Item 추가해주기
	*/
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
	TEnumAsByte<EObjectTypeQuery> Enemy = UEngineTypes::ConvertToObjectType(ECC_Enemy);
	ObjectTypes.Add(Enemy);

	TArray<AActor*> IgnoreActors; // 무시할 액터들.
	FHitResult HitResult; // 히트 결과 값 받을 변수.

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc,
		//FVector(30.f, 30.f, 100.f),
		//camLocation.Rotation(),
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
		IObjectInterface* FindObject = Cast<IObjectInterface>(HitResult.GetActor());
		if (FindObject == nullptr)
			return;

		if(PlayerPawn->HeldAbilityItem == FindObject)
			return;
		if(PlayerPawn->HeldDeadBody == FindObject)
			return;

		LastCharacter = ThisCharacter;
		ThisCharacter = Cast<ABaseCharacter>(HitResult.GetActor());

		if (LastCharacter != ThisCharacter)
		{
			if (LastCharacter)
			{
				LastCharacter->UnHighlightObject(this);
			}
			if (ThisCharacter)
			{

				ThisCharacter->HighlightObject(this);
			}
		}
	}
	else {

		//플레이어의 시선이 닿지 않는다면 아이템의 하이라이트 효과를 꺼주자.
		if (ThisCharacter && IsValid(Cast<UObject>(ThisCharacter)))
		{
			ThisCharacter->UnHighlightObject(this);
			ThisCharacter = nullptr;
		}
	}
}

void AGoblinPlayerController::ScanNearItem()
{
	//TODO
	//리셀 컴퍼니에는 플레이어에게 스캔 능력이 있다. 이걸 우리도 똑같이 만들어줘야 할까?

}

void AGoblinPlayerController::PickUpItem()
{

	if(CanInput == false)
		return;
	CanInput = false;

	GetWorld()->GetTimerManager().SetTimer(InputDelayTimer,
		FTimerDelegate::CreateLambda([&] {
			CanInput = true;
		})
	,0.8f, false);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter == nullptr || ThisObject == nullptr)
		return;

	if (PlayerCharacter->bDead == true)
		return;

	//시체 줍기
	if (ABaseCharacter* BC = Cast<ABaseCharacter>(ThisObject))
	{
		if(BC->bDead == false)
			return;

		PlayerCharacter->TryPickUpDeadBody(BC);
	}


	AItemBase* ThisItem = Cast<AItemBase>(ThisObject);
	if (ThisItem == nullptr)
		return;
	if (ThisItem->ItemData.CanPickUp() == false)
		return;


	PlayerCharacter->TryPickUpItem(ThisItem);
	UGoblinBlueprintFunctionLibrary::DebugLog(FString::Printf(TEXT("%s 가 아이템을 줍습니다."), *PlayerCharacter->GetName()));
}

void AGoblinPlayerController::DropItem()
{

	if (CanInput == false)
		return;
	CanInput = false;

	GetWorld()->GetTimerManager().SetTimer(InputDelayTimer,
		FTimerDelegate::CreateLambda([&] {
			CanInput = true;
			})
		, 0.8f, false);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter == nullptr)
		return;
	if(PlayerCharacter->IsDead() == true)
		return;

	PlayerCharacter->TryDropItem();
	PlayerCharacter->GetInventoryComponent()->Server_RemoveItemDataToDrop();
	UGoblinBlueprintFunctionLibrary::DebugLog(FString::Printf(TEXT("%s 가 아이템을 버립니다."), *PlayerCharacter->GetName()));
}

void AGoblinPlayerController::EquipItem()
{

}

void AGoblinPlayerController::Server_InteractItem_Implementation(AItemBase* Item)
{
	if (Item == nullptr)
		return;

	Item->Interact(this);
}

void AGoblinPlayerController::ShowScript_Implementation(AItemBase* Item)
{
}

void AGoblinPlayerController::CloseScript_Implementation()
{
}

void AGoblinPlayerController::BuyItemInStore_Implementation(int32 Price, FItemData ItemData)
{
	AGoblinGameStateBase* GameState = Cast<AGoblinGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(GameState == nullptr)
	{
		return;
	}

	if (GameState->CheckMoney(Price))
	{
		//AItemBase* BuyItem = GetWorld()->SpawnActor<AItemBase>(ItemData.Model, GetPawn()->GetTransform());
		//BuyItem->SetItemData(ItemData);
		GameState->Server_AddMoney(Price, false);
		GameState->BuyItemInStoreEvent(ItemData);
		Client_BuyItemSuccess(ItemData);
	}
	else		// 소지금 부족
	{
		Client_ShortOfSoul();
	}
}

void AGoblinPlayerController::Client_BuyItemSuccess_Implementation(FItemData ItemData)
{
	BuyItemSuccessEvent(ItemData);
}


void AGoblinPlayerController::Client_ShortOfSoul_Implementation()
{
	ShortOfSoulEvent();
}

// 실행
void AGoblinPlayerController::InteractItem()
{
	AItemBase* ThisItem = Cast<AItemBase>(ThisObject);
	if (ThisItem == nullptr)
		return;
	if(ThisItem->ItemData.HasActionBarTime() == false)
		Server_InteractItem(ThisItem);
	else
		StartActionBar(ThisItem);
}

void AGoblinPlayerController::Jump()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter == nullptr)
		return;

	PlayerCharacter->Jump();
}

AActor* AGoblinPlayerController::GetThisObject()
{
	if(ThisObject == nullptr || IsValid(ThisObject) ==false || ThisObject->IsActorBeingDestroyed() == true)
		return nullptr;

	return ThisObject;
}

void AGoblinPlayerController::Move(const FInputActionValue& InputActionValue)
{
	ABaseCharacter* ControlledPawn = Cast<ABaseCharacter>(GetPawn());
	if (IsValid(ControlledPawn) == false)
		return;
	if(ControlledPawn->bStunned == true)
		return;
	if (ControlledPawn->bDead == true)
		return;

	//사다리를 타고 있지 않을 때 Move
	if (ControlledPawn->OnLadder == false)
	{
		FVector2D MovementVector = InputActionValue.Get<FVector2D>();
		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//Horizontal = MovementVector.X;
		//Vertical = MovementVector.Y;
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("%f %f"), MovementVector.Y, MovementVector.X));
		ControlledPawn->MoveForward = MovementVector.Y;
		ControlledPawn->MoveRight = MovementVector.X;
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);

	}
	else
	{
		FVector2D MovementVector = InputActionValue.Get<FVector2D>();
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector UpDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z) * 100;
		const FVector DownDirection = -FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);

		//지금은 하드 코딩을 해놨는데 추후 필요하면 변수로 뺀다.
		ControlledPawn->GetCharacterMovement()->MaxFlySpeed = 300;
		ControlledPawn->TryChangeFlyingSpeed(300);
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorUpVector(), MovementVector.Y);

	}
}

void AGoblinPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	LookAxisVector *= GameManager->MouseSensivity;

	// add yaw and pitch input to controller
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void AGoblinPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)
		return;

	GetASC()->AbilityInputTagPressed(InputTag);
}

void AGoblinPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
}

void AGoblinPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC())
	{
		GetASC()->AbilityInputTagHeld(InputTag);
	}
}
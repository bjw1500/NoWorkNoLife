// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Inventory/GoblinInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Goblin/BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Goblin/Actors/Item/ItemBase.h"
#include "Goblin/Characters/PlayerCharacter.h"
#include "UI/GoblinHUD.h"
#include "Managers/GameManager.h"

UGoblinInventoryComponent::UGoblinInventoryComponent() : SlotMaxIndex(6)
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UGoblinInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGoblinInventoryComponent, Inventory);
	DOREPLIFETIME(UGoblinInventoryComponent, SlotIndex);
}
void UGoblinInventoryComponent::SetUpItemData(FItemData ItemData)
{
	PickUpItemInfo = ItemData;
	PickUpItemInfo.SlotIndex = SlotIndex;
}

void UGoblinInventoryComponent::Server_AddInventoryArray_Implementation(const FItemData Data, bool IsHeldItem)
{
	FItemData CastItemdata = const_cast<FItemData&>(Data);

	if (!IsHeldItem)
	{
		CastItemdata.SlotIndex = SlotIndex;

		APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
		if (Player != nullptr && Player->HeldObject != nullptr )
		{
			Player->HeldObject->ItemData.SlotIndex = SlotIndex;
		}

		if (!Inventory.IsEmpty())
		{
			for (int i = 0; i < Inventory.Num(); i++)
			{
				if (Inventory[i].SlotIndex == SlotIndex)
				{
					return;
				}
			}
		}
	}
	else
	{
		TArray<int32> CheckInventory;
		CheckInventory.Init(0, 7);
		for (int i = 0; i < Inventory.Num(); i++)
		{
			CheckInventory[Inventory[i].SlotIndex]++;
		}

		for (int i = 0; i < CheckInventory.Num(); i++)
		{
			if (CheckInventory[i] == 0)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
				if (Player == nullptr)
				{
					return;
				}
				for (auto& Item : Inventory)
				{
					if (Item.SlotIndex == SlotIndex && Player->HeldObject != nullptr)
					{
						Item.Duration = Player->HeldObject->Duration;
						break;
					}
				}

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Duration : %f"), Data.Duration));
				CastItemdata.SlotIndex = i;
				SlotIndex = i;
				break;
			}
		}
	}

	Inventory.Add(CastItemdata);
	Client_AddInventoryArray(CastItemdata);
}

bool UGoblinInventoryComponent::Server_AddInventoryArray_Validate(const FItemData Data, bool IsHeldItem)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(GetOwner());


	return true;

	//if (Character->HeldObject->ItemData.Model == Data.Model)
	//{
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}
}

void UGoblinInventoryComponent::Client_AddInventoryArray_Implementation(const FItemData Data)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	if (Player == nullptr)
	{
		return;
	}

	Player->PickUpItemData(Data);
}

void UGoblinInventoryComponent::Server_RemoveItemDataToDrop_Implementation()
{
	// TODO 아이템 떨구기
	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].SlotIndex == SlotIndex)
		{
			Client_RemoveItemDataToDrop(Inventory[i]);
			Inventory.RemoveAt(i);
		}
	}
}

void UGoblinInventoryComponent::Client_RemoveItemDataToDrop_Implementation(const FItemData Data)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	if (Player == nullptr)
	{
		return;
	}

	Player->DropItemData(Data);
}

void UGoblinInventoryComponent::Server_ClearInventoryItemData_Implementation()
{
	Inventory.Empty();
}

void UGoblinInventoryComponent::TestCheckItems_Implementation()
{
	if (Inventory.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("is Empty"));
	}
	for (auto& Item : Inventory)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Item : %s, Slot Index : %d"), *Item.Name.ToString(), Item.SlotIndex));
	}
}

void UGoblinInventoryComponent::Server_SetSlotIndex_Implementation(const FKey& Input)
{
	int32 ConvertInput = FCString::Atoi(*Input.GetDisplayName().ToString());
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	if (Player == nullptr)
	{
		return;
	}

	if (IsValid(Player->HeldObject))
	{
		Player->TryPutInItem();
	}


	SlotIndex = ConvertInput - 1;
	Client_SetSlotIndex(SlotIndex);
}

bool UGoblinInventoryComponent::Server_SetSlotIndex_Validate(const FKey& Input)
{
	int32 ConvertInput = FCString::Atoi(*Input.GetDisplayName().ToString());
	if (ConvertInput > 0 && ConvertInput < SlotMaxIndex)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UGoblinInventoryComponent::Client_SetSlotIndex_Implementation(const int32& Index)
{
	UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameManager == nullptr)
	{
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	if (Player == nullptr)
	{
		return;
	}
	
	if (Index > GameManager->UISlotindex)
	{
		return;
	}

	Player->SetSlotIndexEvent(SlotIndex);
	for (auto Item : Inventory)
	{
		if (Item.SlotIndex == Index)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Index : %d, Model : %s, Item Slot : %d"), Index, *Item.Name.ToString(), Item.SlotIndex));

			Player->GetItemData(Item);
			return;
		}
	}

	Player->GetItemData(FItemData());
}

float UGoblinInventoryComponent::GetDuration()
{
	for (auto Item : Inventory)
	{
		if (Item.SlotIndex == SlotIndex)
		{
			return Item.Duration;
		}
	}
	return 0.0f;
}

// Called when the game starts
void UGoblinInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}
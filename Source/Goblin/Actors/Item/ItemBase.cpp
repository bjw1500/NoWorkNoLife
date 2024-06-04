// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Goblin.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/ItemTextComponent.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = Mesh;
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionObjectType(ECC_Item);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetEnableGravity(true);
	Mesh->SetIsReplicated(true);

	ItemTextPosition = CreateDefaultSubobject<USceneComponent>("ItemTextPosition");
	ItemTextPosition->SetupAttachment(RootComponent);

	Duration = MaxDuration;
	ItemData.SlotIndex = -1;
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AItemBase, Duration, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(AItemBase, ItemOwner, COND_SimulatedOnly);
	DOREPLIFETIME(AItemBase, IsLocked);
	DOREPLIFETIME_CONDITION(AItemBase, ItemData, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(AItemBase, CanSave, COND_SimulatedOnly);
}



void AItemBase::DropItem()
{
	//

}

void AItemBase::HighlightObject(AGoblinPlayerController* Player)
{
	if (IsValid(Mesh) == true)
	{
		Mesh->SetRenderCustomDepth(true);
		Mesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

		//플레이어가 보고 있는 중이니 아이템 정보를 띄워준다.

		ShowItemText();
	}
}

void AItemBase::UnHighlightObject(AGoblinPlayerController* Player)
{
	if (IsValid(Mesh) == true)
	{
		Mesh->SetRenderCustomDepth(false);

		//플레이어가 시선을 뗀다는 소리이니 아이템 정보를 꺼준다.
		CloseItemText();
	}
}


void AItemBase::ShowItemText_Implementation()
{
	if(IsValid(ItemTextComponentClass) == false)
		return;

	ItemTextComponent = NewObject<UItemTextComponent>(this, ItemTextComponentClass);
	ItemTextComponent->RegisterComponent();
	//ItemTextComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	ItemTextComponent->AttachToComponent(ItemTextPosition, FAttachmentTransformRules::KeepRelativeTransform);
	ItemTextComponent->SetItemText(ItemData);
}

void AItemBase::CloseItemText_Implementation()
{
	if(ItemTextComponent == nullptr)
		return;
	if(ItemTextComponent->IsBeingDestroyed() == true)
		return;

	ItemTextComponent->DestroyComponent(true);
}

void AItemBase::Interact_Implementation(AGoblinPlayerController* Player, AActor* Target)
{
	
}

void AItemBase::SnapToSurfaceRandom(bool IfOverlapDetectedRemoveThis)
{
	FVector StartLoc = GetActorLocation();


	FVector EndLoc;

	int32 Rand = FMath::RandRange(0, 3);
	float TempDist = 100000.f;

	if (Rand == 0)
		EndLoc = FVector::RightVector;
	else if (Rand == 1)
		EndLoc = FVector::ForwardVector;
	else if (Rand == 2)
		EndLoc = FVector::LeftVector;
	else if (Rand == 3)
		EndLoc = FVector::BackwardVector;

	FHitResult Result = GetNearObjectDistance(StartLoc, EndLoc);

	if (Result.bBlockingHit == true)
	{
		//TargetList에 있는 이름과 겹칠경우 삭제한다.
		//맵 생성 중에 생겨나는 횃불들이 사다리나 다른 횃불과 겹칠때를 위한 코드.
		if (IfOverlapDetectedRemoveThis == true)
		{
			AItemBase* HitItem = Cast<AItemBase>(Result.GetActor());
			if (HitItem)
			{
				for(FText TargetName : CantOverlapObjectList)
				{
					if (HitItem->ItemData.Name.EqualTo(TargetName))
					{
						Destroy();
						return;
					}
				}
			}
		}

		FVector OffSet = Result.Normal * 10.f;
		Multicast_SetActorLocation(Result.ImpactPoint + OffSet);
	}
	else
	{
		SnapToSurfaceRandom(IfOverlapDetectedRemoveThis);
	}
}

void AItemBase::SnapToSurfaceNear()
{
	FHitResult Result;
	TArray<FVector> DirArray = { FVector::ForwardVector, FVector::BackwardVector, FVector::RightVector, FVector::LeftVector };

	Result = GetNearObjectDistance(GetActorLocation(), FVector::ForwardVector);
	for (FVector Dir : DirArray)
	{
		FHitResult Temp = GetNearObjectDistance(GetActorLocation(), Dir);

		if (Result.Distance > Temp.Distance)
			Result = Temp;
	}

	if (Result.bBlockingHit == true)
	{
		FVector OffSet = Result.Normal * 10.f;
		Multicast_SetActorLocation(Result.ImpactPoint + OffSet);
	}
}

void AItemBase::SnapToSurfaceNearAndRotate(FVector SetLocationOffSet)
{
	FHitResult Result;
	TArray<FVector> DirArray = { FVector::ForwardVector, FVector::BackwardVector, FVector::RightVector, FVector::LeftVector };

	Result = GetNearObjectDistance(GetActorLocation() + 100* FVector::UpVector, FVector::ForwardVector);
	for (FVector Dir : DirArray)
	{
		FHitResult Temp = GetNearObjectDistance(GetActorLocation() + 100 * FVector::UpVector, Dir);

		if (Result.Distance > Temp.Distance)
			Result = Temp;
	}

	if (Result.bBlockingHit == true)
	{
		FVector OffSet = Result.Normal * 10.f;
		FRotator NormalRotator = UKismetMathLibrary::MakeRotFromX(Result.ImpactNormal);

		Multicast_SetActorRotation(NormalRotator);
		Multicast_SetActorLocation(Result.ImpactPoint + OffSet +SetLocationOffSet);
	}
}

void AItemBase::SnapToGround()
{
	FHitResult Result = GetNearObjectDistance(GetActorLocation(), FVector::DownVector);

	if (Result.bBlockingHit == true)
	{
		FVector OffSet = Result.Normal;
		//SetActorLocation(Result.ImpactPoint + OffSet);
		Multicast_SetActorLocation(Result.ImpactPoint + OffSet, true);
	}
}

bool AItemBase::SnapToObject(FVector StartLoc, FVector Dir, bool IfOverlapDetectedRemoveThis)
{
	FHitResult Result = GetNearObjectDistance(StartLoc, Dir);
	TArray<FVector> DirArray = { FVector::ForwardVector, FVector::BackwardVector, FVector::RightVector, FVector::LeftVector };

	if (Result.bBlockingHit == true)
	{
		if (IfOverlapDetectedRemoveThis)
		{
			for(FVector Dir : DirArray)
			{ 
				FHitResult DetectResult = GetNearObjectDistance(StartLoc, Dir);
				if (DetectResult.Distance < 10)
				{
					Destroy();
					return false;
				}
			}
		}

		FVector OffSet = Result.Normal * 10.f;
		Multicast_SetActorLocation(Result.ImpactPoint + OffSet);
		return true;
	}
	return false;
}

FHitResult AItemBase::GetNearObjectDistance(FVector StartLoc, FVector Dir)
{
	float TempDist = 1000000.f;
	FVector EndLoc = StartLoc + TempDist * Dir;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
	TEnumAsByte<EObjectTypeQuery> MapObject = UEngineTypes::ConvertToObjectType(ECC_MapObject);
	//ObjectTypes.Add(WorldStatic);
	//ObjectTypes.Add(WorldDynamic);
	ObjectTypes.Add(MapObject);

	TArray<AActor*> IgnoreActors; // 무시할 액터들.
	IgnoreActors.Add(this);

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
	);

	return HitResult;
}

void AItemBase::IsLockedEvent_Implementation()
{
}

void AItemBase::Multicast_SetActorRotation_Implementation(FRotator Rotation)
{
	SetActorRotation(Rotation);
}

void AItemBase::Multicast_SetActorLocation_Implementation(FVector Location, bool InitRotation)
{
	SetActorLocation(Location);

	if(InitRotation)
		SetActorRotation(FRotator::ZeroRotator);
}

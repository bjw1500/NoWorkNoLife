// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemClassInfo.h"
#include "AbilitySystemComponent.h"
#include "Interface/ObjectInterface.h"
#include "ItemBase.generated.h"


/*
	플레이어가 주울 수 있는 오브젝트의 최상위.
	무기, 아이템, 고블린이 주울 기타 아이템

	ItemBase
		Weapon
		Consumable
		Armor?
		
*/

class USkeletalMeshComponent;
class UGameplayAbility;
class UItemTextComponent;
class APlayerController;
class AGoblinPlayerController;
class ABaseCharacter;
class USoundCue;

UCLASS()
class GOBLIN_API AItemBase : public AActor, public IObjectInterface
{
	GENERATED_BODY()
	
public:	
	AItemBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

public:	

	virtual void DropItem();																												
	virtual void HighlightObject(AGoblinPlayerController* Player);
	virtual void UnHighlightObject(AGoblinPlayerController* Player);

	UFUNCTION(Client, Reliable)
	void ShowItemText();
	UFUNCTION(Client, Reliable)
	void CloseItemText();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetWidget();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowWidget();
	
	UFUNCTION(BlueprintImplementableEvent)
	void HideWidget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(AGoblinPlayerController* Player = nullptr, AActor* Target = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void SnapToSurfaceRandom(bool IfOverlapDetectedRemoveThis = false);									//근처에 있는 아무 오브젝트에 붙인다
	UFUNCTION(BlueprintCallable)
	virtual void SnapToSurfaceNear();											//동서남북으로 가까이 있는 오브젝트에 붙인다.
	UFUNCTION(BlueprintCallable)
	virtual void SnapToSurfaceNearAndRotate(FVector OffSet);		
	UFUNCTION(BlueprintCallable)
	virtual void SnapToGround();		
	UFUNCTION(BlueprintCallable)
	virtual bool SnapToObject(FVector StartLoc,FVector Dir, bool IfOverlapDetectedRemoveThis = false);
	UFUNCTION(BlueprintCallable)
	virtual FHitResult GetNearObjectDistance(FVector StartLoc, FVector Dir);	//해당 방향으로 까까운 오브젝트를 찾는다.
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetActorLocation(FVector Location, bool InitRotation = false);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetActorRotation(FRotator Rotation);


	void				SetItemData(FItemData Data){ItemData =Data; }
	FItemData	GetItem(){return ItemData;}

	UFUNCTION(BlueprintCallable)
	void				SetObjectScript(FText Script){ItemData.ObjectScript = Script;};
	UFUNCTION(BlueprintCallable)
	FText				GetObjectScript(){return ItemData.ObjectScript;}

	USkeletalMeshComponent* GetMesh(){return Mesh;}

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DissolveDestroy();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goblin | Item")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, Replicated,BlueprintReadWrite, Category = "Goblin | Item")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goblin | Item")
	TArray<FText> CantOverlapObjectList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goblin | Item")
	TObjectPtr<USkeletalMeshComponent> Mesh;			

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Goblin | Item")
	TObjectPtr<ABaseCharacter> ItemOwner;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goblin | Item")
	TObjectPtr<USceneComponent> ItemTextPosition;			
	
	UPROPERTY()
	TObjectPtr<UItemTextComponent> ItemTextComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Goblin | Item")
	TSubclassOf<UItemTextComponent> ItemTextComponentClass;

	UPROPERTY()
	FGameplayAbilitySpecHandle	 ItemAbilitySpecHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goblin | Item")
	TObjectPtr<USoundCue> InteractSoundCue;			
	
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Goblin | Item")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Goblin | Item")
	bool IsLocked = false;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IsLockedEvent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Goblin | Item")
	bool CanSave = false;

	UPROPERTY(BlueprintReadWrite, Category = "Goblin | Item")
	float MaxDuration = 200;
};

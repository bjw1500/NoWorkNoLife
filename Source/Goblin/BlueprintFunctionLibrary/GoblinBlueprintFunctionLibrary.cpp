// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"
#include "GameMode/GoblinGameModeBase.h"
#include "GAS/GoblinAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/GoblinPlayerState.h"
#include "Interface/CombatInterface.h"
#include "UI/GoblinHUD.h"
#include "Characters/CharacterClassInfo.h"
#include "Player/GoblinPlayerController.h"

void UGoblinBlueprintFunctionLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	/*
		왜 굳이 GameMode에서 가져오는가?
		데디서버에서 GameMode는 서버만 가지고 있는 객체이다.
		그래서 게임 전반적으로 서버에서만 다뤄야하는 정보는 GameMode에서 가지고 있는게 좋다.
	*/
	AGoblinGameModeBase* GM = Cast<AGoblinGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GM == nullptr)
		return;

	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GM->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle MainStatAttributeContextHandle = ASC->MakeEffectContext();
	MainStatAttributeContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle MainStatAttributeSpecHandle = ASC->MakeOutgoingSpec(
		ClassDefaultInfo.MainStatAttributes, Level, MainStatAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*MainStatAttributeSpecHandle.Data.Get());
}

void UGoblinBlueprintFunctionLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	AGoblinGameModeBase* GM = Cast<AGoblinGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GM == nullptr)
		return;

	/*
		Common Ability			공용적인 어빌리티들.
		ex)HitReaction.
	*/

	UCharacterClassInfo* CharacterClassInfo = GM->CharacterClassInfo;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}


	/*
		CharacterClass Ability	캐릭터 고유 스킬들
	*/
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	UGoblinAbilitySystemComponent* GASC = Cast<UGoblinAbilitySystemComponent>(ASC);
	GASC->AddCharacterAbilities(DefaultInfo.StartupAbilities);
	
	
	//for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	//{
	//	ICombatInterface* Interface = Cast<ICombatInterface>(ASC->GetAvatarActor());
	//	if (Interface)
	//	{
	//		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, Interface->GetLevel());
	//		ASC->GiveAbility(AbilitySpec);
	//	}
	//}

	

}

UCharacterClassInfo* UGoblinBlueprintFunctionLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AGoblinGameModeBase* GM = Cast<AGoblinGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GM == nullptr)
		return nullptr;

	return GM->CharacterClassInfo;
}

void UGoblinBlueprintFunctionLibrary::DebugLog(FString string)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, *string);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *string);
	}
}

void UGoblinBlueprintFunctionLibrary::GetLiveObjectWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	// query scene to see what we hit
	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult Overlap : Overlaps)
		{
			//확인결과 ICombatInterface가 없으면 Return
			ICombatInterface* Interface = Cast<ICombatInterface>(Overlap.GetActor());
			if(Interface == nullptr)
				continue;;

			//살아있다면 Overlap 결과물에 넣어준다.
			//BlueprintNative 속성이 있을 경우 Interface에서 바로 사용할 수 없게 된다.
			//check(0 && "Do not directly call Event functions in Interfaces. Call Execute_IsDead instead.");
			//대신 static 계열의 Execute를 사용해줘야 한다.
			const bool IsAlive = !ICombatInterface::Execute_IsDead(Overlap.GetActor());
			if (IsAlive == true)
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}

	}
}

bool UGoblinBlueprintFunctionLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	if(FirstActor == nullptr || SecondActor == nullptr)
		return false;

	const bool BothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool BothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool BothAreAdventurer = FirstActor->ActorHasTag(FName("Adventurer")) && SecondActor->ActorHasTag(FName("Adventurer"));

	const bool bFriends = BothArePlayers || BothAreEnemies || BothAreAdventurer;

	return !bFriends;

}

UGoblinUserWidget* UGoblinBlueprintFunctionLibrary::GetInGameUI(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AGoblinHUD* HUD = Cast<AGoblinHUD>(PC->GetHUD()))
		{
			return HUD->InGameUI_Widget;
		}
	}
	return nullptr;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Characters/CharacterClassInfo.h"
#include "GoblinBlueprintFunctionLibrary.generated.h"

/**
 * 
 */

 class AGoblinGameModeBase;
 class AGoblinPlayerController;

UCLASS()
class GOBLIN_API UGoblinBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "RPGBlueprintLibrary | Attribute")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "RPGBlueprintLibrary | CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "RPGBlueprintLibrary | CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "RPGBlueprintLibrary | Utils")
	static void DebugLog(FString string);

	UFUNCTION(BlueprintCallable, Category = "RPGBlueprintLibrary | GameplayMechanic")
	static void GetLiveObjectWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "RPGBlueprintLibrary | GameplayMechanic")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);


	/*
		UI
	*/

	UFUNCTION(BlueprintCallable, Category = "RPGBlueprintLibrary | UI")
	static UGoblinUserWidget* GetInGameUI(const UObject* WorldContextObject);
};

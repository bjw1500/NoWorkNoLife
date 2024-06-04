// Fill out your copyright notice in the Description page of Project Settings.

#include "GoblinAssetManager.h"
#include "GameplayTagManager.h"
#include "AbilitySystemGlobals.h"

UGoblinAssetManager& UGoblinAssetManager::Get()
{
	check(GEngine);

	UGoblinAssetManager* Manager = Cast<UGoblinAssetManager>(GEngine->AssetManager);
	return *Manager;
}

void UGoblinAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FGameplayTagManager::Init();
	//UAbilitySystemGlobals::Get().InitGlobalData();
}

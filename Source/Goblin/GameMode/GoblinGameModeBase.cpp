// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GoblinGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void AGoblinGameModeBase::GetSeamlessTravelActorList(bool bToEntry, TArray<class AActor*>& ActorList)
{

	AddTravelActorList(ActorList);

	for (AActor* At : TestList)
	{
		ActorList.Add(At);
	}
	Super::GetSeamlessTravelActorList(bToEntry, ActorList);
}

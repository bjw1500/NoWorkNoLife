// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinPlayerState.h"
#include "GAS/GoblinAbilitySystemComponent.h"
#include "GAS/GoblinAttributeSet.h"
#include "Net/UnrealNetwork.h"

AGoblinPlayerState::AGoblinPlayerState()
{
	//멀티플레이 상황에서 네트워크 업데이트 간격 설정.
	NetUpdateFrequency = 100;

	AbilitySystemComponent = CreateDefaultSubobject<UGoblinAbilitySystemComponent>("AbilitySystemComponent");
	//Rep 옵션이 켜져야 멀티에 필요한 정보들이 Rep 된다.
	AbilitySystemComponent->SetIsReplicated(true);

	/*
	ReplicationMode :: Mixed
	Gameplay Effect는 소유한 클라이언트에게만 복제가 된다.
	Gameplay Cues나 Gameplay Tags는 모든 클라이언트한테 복제가 된다.
*/
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UGoblinAttributeSet>("AttributeSet");
}

void AGoblinPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//복제될 Rep 변수들 등록
	DOREPLIFETIME(AGoblinPlayerState, Level);
}

UAbilitySystemComponent* AGoblinPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGoblinPlayerState::OnRep_Level(int32 OldLevel)
{
		//아직은 레벨 변화에 따른 이벤트를 넣어줄게 없어서 비어둔다.

}

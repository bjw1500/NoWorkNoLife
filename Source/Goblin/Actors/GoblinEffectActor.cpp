// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinEffectActor.h"
#include "AbilitySystemInterface.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/GoblinAttributeSet.h"

// Sets default values
AGoblinEffectActor::AGoblinEffectActor()
{
	//함정이나 트랩으로 나중에TICk 사용할게 생기면 따로 하위클래스 만들어서 켜주기.
	//지금은 Tick을 꺼둔다.
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent")));
}

// Called when the game starts or when spawned
void AGoblinEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGoblinEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(TargetActor);
	if (Interface)
	{
		UAbilitySystemComponent* ASC = Interface->GetAbilitySystemComponent();

		check(GameplayEffectClass);

		/*
			GameplayEffect를 생성해서 상대방한테 적용한다
			1.적용할 Target의 Effect의 정보를 가져와야 한다.
			2.적용할 Effect 정보와 Target의 Effect 정보를 합칠 구조체를 만든다.
			3.준비가 되었다면 Target의 AbilitySystemComponent에다가 만든 구조체를 넘겨서 Effect를 적용시킨다.
		*/
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
		const FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		/*
			EffectSpecHandle을 통해 적용한 GE를 관리할 수가 있다.
		*/
		const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
		if (bIsInfinite && InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
		{
			//Instant나 Duration 타입 경우 알아서 사라진다.
			//하지만 Infinite 경우 플레이어가 직접 제거해주지 않으면 계속 지속된다.
			//그렇기에 제거할때를 위해 TMap 유형으로 데이터를 저장해준다.
			ActiveEffectHandles.Add(ActiveEffectHandle, ASC);
		}

	}
}

void AGoblinEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AGoblinEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (IsValid(TargetASC) == false)
			return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}

	}
}


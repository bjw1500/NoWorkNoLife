// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Managers/GameplayTagManager.h"
#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"
#include "Interface/CombatInterface.h"
#include "Characters/BaseCharacter.h"
#include "Characters/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UGoblinAttributeSet::UGoblinAttributeSet()
{
}

void UGoblinAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//Replicate 되려는 값들을 등록해준다.
	DOREPLIFETIME_CONDITION_NOTIFY(UGoblinAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGoblinAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGoblinAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGoblinAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGoblinAttributeSet, CurrentMoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGoblinAttributeSet, BaseMoveSpeed, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGoblinAttributeSet, SearchRange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGoblinAttributeSet, HearingRange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGoblinAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGoblinAttributeSet, OpenDoorSpeed, COND_None, REPNOTIFY_Always);
}

void UGoblinAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	/*
		속성의 현재 값에 대한 변경사항을 제일 먼저 알려오는 곳.
		NewValue 값을 바꾸어도 실제 값에는 영향을 못주니, 값 변경 전에 뭔가 할 게 있을 때만 사용하도록 하자.

		1. PreAttributeBaseChange(여기서 NewValue로 BaseValue에 영향을 줄 수 있음, 즉 클램핑)
		2. PreAttributeChange(여기서 NewValue로 CurrentValue에 영향을 줄 수 있음, 즉 클램핑)
		3. PostGameplayEffectsExecute(BaseValue가 변경되기 직전에 호출됨) .클램핑하기 좋은 지점)

		따라서 현재 Current 및 Base 값을 모두 고정하는 가장 좋은 방법은
		PreAttributeBaseChange() 및 PreAttributeChange() 메서드에 동일한 고정 논리를 추가하는 것 같습니다.
		그리고 클램핑 없이 PostGameplayEffectExecute()를 그대로 두는 것이 더 좋습니다.
	*/

	//if (Attribute == GetHealthAttribute())
	//{
	//	NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	//}
	//if (Attribute == GetManaAttribute())
	//{
	//	NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	//}
}

void UGoblinAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	/*
		GameplayEffect로 인해 데이터 변경 후에 호출되는 곳.
		그런데 이건 서버에서 실행이 되는가? 클라에서 실행이 되는가?
	*/

	//Data에서 우리가 사용하기 편하게 필요한 정보를 빼네 FEffectProperties 구조체 안에  담는다.
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		//UE_LOG(LogTemp, Warning, TEXT("Changed Health on %s, Health : %f"), *Props.TargetAvatarActor->GetName(), GetHealth());
	}

		ABaseCharacter* BC = Cast<ABaseCharacter>(Props.TargetAvatarActor);
		if (BC == nullptr)
			return;
		if (BC->CharacterMoveState == ECharacterMoveState::Run && BC->GetVelocity().Length() <= 30)
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FGameplayTagManager::Get().Effects_StopRunning);
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
		}
		
		if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
		{
			SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
			//UE_LOG(LogTemp, Warning, TEXT("Changed Health on %s, Health : %f"), *Props.TargetAvatarActor->GetName(), GetHealth());

			if (GetStamina() <= 1)
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FGameplayTagManager::Get().Effects_StopRunning);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

	}

	//이동속도 변화
	if (Data.EvaluatedData.Attribute == GetCurrentMoveSpeedAttribute())
	{
		 if(BC == nullptr)
			return;

		 if (BC->HasAuthority())
			 BC->Server_ChangeSpeed(GetCurrentMoveSpeed());
	}


	if (Data.EvaluatedData.Attribute == GetOpenDoorSpeedAttribute())
	{
		AEnemyCharacter* EC = Cast<AEnemyCharacter>(Props.TargetAvatarActor);
		if (EC == nullptr)
			return;

		EC->OpenDoorTime = GetOpenDoorSpeed();
	}

	/*
		IncomingDamageAttribute는 복제 되지 않는 Attribute이다.
		대신 서버에서 데미지 계산이 이루어지고, 여기서 나오는 결과들이(데미지 받은 결과 hp가 얼마가 되었다~) 클라이언트에 Rep된다.
	*/
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
	//	UGoblinBlueprintFunctionLibrary::DebugLog(FString::Printf(TEXT("InComingDamage:%f"), LocalIncomingDamage));
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;

			//사망했다면?
			if (bFatal == true)
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}

			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FGameplayTagManager::Get().Effects_HitReact_Common);
				//해당 태그와 맞는 Ability가 있다면 활성화한다.
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

		}
	}
}

void UGoblinAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	//Source
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}

		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	//Target
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);

		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);

	}
}

void UGoblinAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGoblinAttributeSet, Health, OldHealth);
}

void UGoblinAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGoblinAttributeSet, MaxHealth, OldMaxHealth);
}

void UGoblinAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGoblinAttributeSet, Stamina, OldStamina);
}

void UGoblinAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGoblinAttributeSet, MaxStamina, OldMaxStamina);
}

void UGoblinAttributeSet::OnRep_BaseMoveSpeed(const FGameplayAttributeData& OldBaseMoveSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGoblinAttributeSet, BaseMoveSpeed, OldBaseMoveSpeed);
}

void UGoblinAttributeSet::OnRep_CurrentMoveSpeed(const FGameplayAttributeData& OldCurrentSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGoblinAttributeSet, CurrentMoveSpeed, OldCurrentSpeed);
}

void UGoblinAttributeSet::OnRep_SearchRange(const FGameplayAttributeData& OldSearchRange) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGoblinAttributeSet, SearchRange, OldSearchRange);
}

void UGoblinAttributeSet::OnRep_HearingRange(const FGameplayAttributeData& OldHearingRange) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGoblinAttributeSet, HearingRange, OldHearingRange);
}

void UGoblinAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGoblinAttributeSet, AttackSpeed, OldAttackSpeed);
}

void UGoblinAttributeSet::OnRep_OpenDoorSpeed(const FGameplayAttributeData& OldOpenDoorSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGoblinAttributeSet, OpenDoorSpeed, OldOpenDoorSpeed);
}

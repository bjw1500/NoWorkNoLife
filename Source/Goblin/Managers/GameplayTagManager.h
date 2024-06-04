// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagsManager.h"

/**`
	Gameplay Tag를 관리하기 위한 구조체겸 싱글톤
 */

struct FGameplayTagManager
{
public:
	static const FGameplayTagManager& Get() { return GameplayTags; }

	/*Init은 GolbinAssetManager에서 진행이 된다.*/
	static void	Init();

	/*Attributes*/
	FGameplayTag Attributes_MainStat_Health;
	FGameplayTag Attributes_MainStat_MaxHealth;
	FGameplayTag Attributes_MainStat_Stamina;
	FGameplayTag Attributes_MainStat_MaxStamina;
	FGameplayTag Attributes_MainStat_MoveSpeed;
	FGameplayTag Attributes_MainStat_SearchRange;
	FGameplayTag Attributes_MainStat_HearingRange;
	FGameplayTag Attributes_MainStat_AttackSpeed;
	FGameplayTag Attributes_MainStat_OpenDoorTime;

	/*Input*/
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Shift;
	FGameplayTag InputTag_Ctrl;

	/*Damage*/
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Ice;
	FGameplayTag Damage_Wind;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Rock;
	FGameplayTag Damage_Physical;
	TArray<FGameplayTag> DamageTypes;

	/*Effect*/
	FGameplayTag Effects_HitReact;
	FGameplayTag Effects_HitReact_Common;
	FGameplayTag Effects_HitReact_NoHead;
	FGameplayTag Effects_HitReact_NoLeft;
	FGameplayTag Effects_HitReact_NoLowBody;
	FGameplayTag Effects_HitReact_NoBody;
	FGameplayTag Effects_HitReact_Warrior;


	FGameplayTag Effects_StunReact;
	FGameplayTag Effects_FallingReact;
	FGameplayTag Effects_StopRunning;

	/*Ability*/
	FGameplayTag Ability_Attack;
	FGameplayTag Ability_Run;
	FGameplayTag Ability_Walk;
	FGameplayTag Ability_Crouch;
	FGameplayTag Ability_Skill_Rush;
	FGameplayTag Ability_Skill_FireBolt;
	FGameplayTag Ability_Skill_Stealth;
	FGameplayTag Ability_Skill_Kidnap;
	FGameplayTag Ability_Skill_MindControll;
	FGameplayTag Ability_Skill_NecroRevive;
	FGameplayTag Ability_Skill_HarpoonAttack;
	FGameplayTag Ability_Skill_Gluttony;
	FGameplayTag Ability_Skill_AllyAssistance;
	FGameplayTag Ability_Skill_UnionSlime;
	FGameplayTag Ability_Skill_ElysiaAction;
	FGameplayTag Ability_Skill_ElysiaSmile;
	FGameplayTag Ability_Skill_ElysiaHello;

	/*Event*/
	FGameplayTag Montage_Skill_FireBolt;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	FGameplayTag Montage_Skill_Rush;
	FGameplayTag Montage_Skill_Stealth;
	FGameplayTag Montage_Skill_Kidnap;
	FGameplayTag Montage_Skill_MindControll;
	FGameplayTag Montage_Skill_NecroRevive;
	FGameplayTag Montage_Skill_HarpoonAttack;
	FGameplayTag Montage_Skill_Gluttony;
	FGameplayTag Montage_Skill_AllyAssistance;
	FGameplayTag Montage_Skill_UnionSlime;
	FGameplayTag Montage_Skill_Start;
	FGameplayTag Montage_Skill_End;
	FGameplayTag Montage_Skill_ElysiaAction;
	FGameplayTag Montage_Skill_ElysiaSmile;
	FGameplayTag Montage_Skill_ElysiaHello;


	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;


	/*State*/
	FGameplayTag State_Idle;
	FGameplayTag State_Walk;
	FGameplayTag State_Jump;
	FGameplayTag State_Run;
	FGameplayTag State_Crouch;

	/*Gameplay Cue*/
	FGameplayTag GameplayCue_MeleeImpact;
	FGameplayTag GameplayCue_SoundImpact;

private:
	static FGameplayTagManager GameplayTags;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagManager.h"

FGameplayTagManager FGameplayTagManager::GameplayTags;

void FGameplayTagManager::Init()
{
	GameplayTags.Attributes_MainStat_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.MainStat.Health"), FString(TEXT("체력")));
	GameplayTags.Attributes_MainStat_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.MainStat.MaxHealth"), FString(TEXT("최대 체력")));
	GameplayTags.Attributes_MainStat_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.MainStat.Stamina"), FString(TEXT("스태미나.")));
	GameplayTags.Attributes_MainStat_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.MainStat.MaxStamina"), FString(TEXT("최대 스태미나.")));
	GameplayTags.Attributes_MainStat_MoveSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.MainStat.MoveSpeed"), FString(TEXT("이동속도")));

	GameplayTags.Attributes_MainStat_SearchRange = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.MainStat.SearchRange"), FString(TEXT("탐색거리")));
	GameplayTags.Attributes_MainStat_HearingRange = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.MainStat.Hearing"), FString(TEXT("청각 거리")));
	GameplayTags.Attributes_MainStat_AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.MainStat.AttackSpeed"), FString(TEXT("공격속도")));
	GameplayTags.Attributes_MainStat_OpenDoorTime= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.MainStat.OpenDoorTime"), FString(TEXT("공격속도")));

	/*Input*/
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.LMB"), FString(TEXT("마우스 왼쪽 버튼")));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.RMB"), FString(TEXT("마우스 오른쪽 버튼")));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.1"), FString(TEXT("1")));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.2"), FString(TEXT("2")));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.3"), FString(TEXT("3")));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.4"), FString(TEXT("4")));
	GameplayTags.InputTag_Shift = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Shift"), FString(TEXT("Shift")));
	GameplayTags.InputTag_Ctrl= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Ctrl"), FString(TEXT("Ctrl")));

	/*Damage*/
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString(TEXT("Damage")));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString(TEXT("Damage")));
	//GameplayTags.Damage_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Ice"), FString(TEXT("Damage")));
	//GameplayTags.Damage_Wind = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Wind"), FString(TEXT("Damage")));
	//GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"), FString(TEXT("Damage")));
	//GameplayTags.Damage_Rock = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Rock"), FString(TEXT("Damage")));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString(TEXT("Damage")));
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Fire);

	/*Effect*/
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString(TEXT("Hit React")));
		GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString(TEXT("Hit React")));
	GameplayTags.Effects_StunReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.StunReact"), FString(TEXT("Stun React")));
	GameplayTags.Effects_FallingReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.FallingReact"), FString(TEXT("Falling React")));
	GameplayTags.Effects_StopRunning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.StopRunning"), FString(TEXT("StopRunning")));

	GameplayTags.Effects_HitReact_Common = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact.Common"), FString(TEXT("Hit React")));
	GameplayTags.Effects_HitReact_NoHead = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact.NoHead"), FString(TEXT("Hit React")));
	GameplayTags.Effects_HitReact_NoLeft = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact.NoLeft"), FString(TEXT("Hit React")));
	GameplayTags.Effects_HitReact_NoLowBody = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact.NoLowBody"), FString(TEXT("Hit React")));
	GameplayTags.Effects_HitReact_Warrior = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact.Warrior"), FString(TEXT("Hit React")));
	GameplayTags.Effects_HitReact_NoBody = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact.NoBody"), FString(TEXT("Hit React")));
	/*Ability*/
	GameplayTags.Ability_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Attack"), FString(TEXT("Attack")));
	GameplayTags.Ability_Run = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Run"), FString(TEXT("Run")));
	GameplayTags.Ability_Walk = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Walk"), FString(TEXT("Walk")));
	GameplayTags.Ability_Crouch = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Crouch"), FString(TEXT("Crouch")));
	GameplayTags.Ability_Skill_Stealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill.Stealth"), FString(TEXT("Stealth")));
	GameplayTags.Ability_Skill_Kidnap = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill.Kidnap"), FString(TEXT("Kidnap")));
	GameplayTags.Ability_Skill_Rush = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill.Rush"), FString(TEXT("Rush")));
	GameplayTags.Ability_Skill_MindControll = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill.MindControll"), FString(TEXT("MindControll")));
	GameplayTags.Ability_Skill_NecroRevive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill.NecroRevive"), FString(TEXT("NecroRevive")));
	GameplayTags.Ability_Skill_FireBolt= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill.FireBolt"), FString(TEXT("FireBolt")));
	GameplayTags.Ability_Skill_HarpoonAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill.HarpoonAttack"), FString(TEXT("HarpoonAttack")));
	GameplayTags.Ability_Skill_Gluttony = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill.Gluttony"), FString(TEXT("Gluttony")));
	GameplayTags.Ability_Skill_AllyAssistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill.AllyAssistance"), FString(TEXT("AllyAssistance")));
	GameplayTags.Ability_Skill_UnionSlime = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill.UnionSlime"), FString(TEXT("UnionSlime")));
	GameplayTags.Ability_Skill_ElysiaAction = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill.ElysiaAction"), FString(TEXT("ElysiaAction")));
	GameplayTags.Ability_Skill_ElysiaSmile = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill.ElysiaSmile"), FString(TEXT("ElysiaSmile")));
	GameplayTags.Ability_Skill_ElysiaHello = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill.ElysiaHello"), FString(TEXT("ElysiaHello")));

	/*Montage*/
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"), FString(TEXT("Attack Socket")));
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"), FString(TEXT("Right Hand Socket")));
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"), FString(TEXT("Left Hand Socket")));


	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString(TEXT("Montage Attack 1")));
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString(TEXT("Montage Attack 2")));
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString(TEXT("Montage Attack 3")));
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString(TEXT("Montage Attack 4")));

	GameplayTags.Montage_Skill_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.FireBolt"), FString(TEXT("FireBolt")));
	GameplayTags.Montage_Skill_Rush = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.Rush"), FString(TEXT("Montage Skill Rush")));
	GameplayTags.Montage_Skill_Stealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.Stealth"), FString(TEXT("Stealth")));
	GameplayTags.Montage_Skill_Kidnap = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.Kidnap"), FString(TEXT("Kidnap")));
	GameplayTags.Montage_Skill_MindControll = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.MindControll"), FString(TEXT("MindControll")));
	GameplayTags.Montage_Skill_NecroRevive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.NecroRevivel"), FString(TEXT("NecroRevivel")));
	GameplayTags.Montage_Skill_HarpoonAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.HarpoonAttack"), FString(TEXT("HarpoonAttack")));
	GameplayTags.Montage_Skill_Gluttony= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.Gluttony"), FString(TEXT("Gluttony")));
	GameplayTags.Montage_Skill_AllyAssistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.AllyAssistance"), FString(TEXT("AllyAssistance")));
	GameplayTags.Montage_Skill_UnionSlime = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.UnionSlime"), FString(TEXT("UnionSlime")));
	GameplayTags.Montage_Skill_ElysiaAction = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.ElysiaAction"), FString(TEXT("Elysia Action")));
	GameplayTags.Montage_Skill_ElysiaSmile = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.ElysiaSmile"), FString(TEXT("Elysia Smile")));
	GameplayTags.Montage_Skill_ElysiaHello = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.ElysiaHello"), FString(TEXT("Elysia Hello")));
	GameplayTags.Montage_Skill_End = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.End"), FString(TEXT("Montage Skill End")));
	GameplayTags.Montage_Skill_Start = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Skill.Start"), FString(TEXT("Montage Skill Start")));
	/*State*/

	GameplayTags.State_Idle = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Idle"), FString(TEXT("Idle")));
	GameplayTags.State_Walk = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Walk"), FString(TEXT("Walk")));
	GameplayTags.State_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Jump"), FString(TEXT("Jump")));
	GameplayTags.State_Run = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Run"), FString(TEXT("Run")));
	GameplayTags.State_Crouch = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Crouch"), FString(TEXT("Crouch")));

	/*GameplayCue*/
	GameplayTags.GameplayCue_MeleeImpact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.MeleeImpact"), FString(TEXT("GameplayCue MeleeImpact")));
	GameplayTags.GameplayCue_SoundImpact= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.SoundImpact"), FString(TEXT("GameplayCue SoundImpact")));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileGameplayAbility.h"
#include "Actors/GoblinProjectile.h"
#include "Interface/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Managers/GameplayTagManager.h"
#include "Components/ArrowComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UProjectileGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UProjectileGameplayAbility::SpawnProjectile(const FVector& ProjectileSpawnLocation, const FVector& ProjectileTargetLocation, bool HasTarget)
{
	/*
		투사체 생성하는 Ability. 그러니 투사체에 Replicates 옵션을 켜주자.
		서버와 클라이언트 구분을 잘해줘야 한다.
	*/

	//클라이언트라면 return
	//오브젝트 스폰은 서버에서 담당해야 한다. 
	//그리고 스폰되면, Client한테 Rep 되도록 해주자.
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (bIsServer == false)
		return;

	/*
		GetOwningActorFromActorInfo
		Ability를 소유한 Actor의 정보를 토해낸다.
	*/
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		
		//투사체 생성을 어디서 해야하는가?
		//지금은 무기에 설정해놓은 Socket에서 생성되게 해주자.
		//FVector SocketLocation = CombatInterface->GetWeaponSocketLocation();
		FVector SocketLocation = ProjectileSpawnLocation;
		FRotator Rotation = FRotator::ZeroRotator;
		if (FVector::Zero() == SocketLocation)
		{
			//SocketLocation == FVector::Zero 라면 무기가 없다는 소리.
			//무기가 없을 경우 투사체를 날리는 Ability를 사용할 때 어떻게 할지는 추후 고민.
			UArrowComponent* ArrowComponent = GetAvatarActorFromActorInfo()->GetComponentByClass<UArrowComponent>();
			SocketLocation = ArrowComponent->GetComponentLocation();
			Rotation = GetAvatarActorFromActorInfo()->GetActorForwardVector().ToOrientationRotator();

			if (HasTarget)
			{
				Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
			}
		}else
		{
			//타겟이 없으면, 혹은 논타겟팅 스킬을 사용했다면 사용자가 바라보는 방향으로 발사
			if (HasTarget == false)
			{
				Rotation = GetAvatarActorFromActorInfo()->GetActorForwardVector().ToOrientationRotator();
				Rotation.Pitch = 0.f;
			}
			else
			//타겟이 존재하면 타겟이 있는 방향으로 발사.
				Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	
		}

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AGoblinProjectile* Projectile = GetWorld()->SpawnActorDeferred<AGoblinProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetAvatarActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		/*
				Damage
		*/
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();

		/***Temp Test***/
		//EffectContext에는 다양한 정보를 담을 수 있다.
		//그런데 다른 정보를 더 넣고 싶다면 EffectContext의 하위 클래스를 만들어서 필요한 정보를 담자.
		//EffectContextHandle.SetAbility(this);
		//EffectContextHandle.AddSourceObject(Projectile);
		//TArray<TWeakObjectPtr<AActor>> Actors;
		//Actors.Add(Projectile);
		//EffectContextHandle.AddActors(Actors);
		//FHitResult HitResult;
		//EffectContextHandle.AddHitResult(HitResult);

		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
		for (auto& Pair : DamageTypes)
		{
			const float ScaleDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaleDamage);
		}


		//생성한 GameplayEffect의 Handle 값을 넘겨주면서 투사체를 소환한다.
		Projectile->DamageEffectSpecHandle = SpecHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}
}

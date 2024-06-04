// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinProjectile.h"
#include "Goblin.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BlueprintFunctionLibrary/GoblinBlueprintFunctionLibrary.h"
#include "Characters/EnemyCharacter.h"

// Sets default values
AGoblinProjectile::AGoblinProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AGoblinProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AGoblinProjectile::OnSphereOverlap);

	if(LoopingSound)
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AGoblinProjectile::Destroyed()
{
	//아무대도 부딪히지 않았다면, 클라이언트측에서 임팩트와 소리를 내고 파괴.
	//서버는 굳이 이펙트를 재생할 필요가 없으므로 패스.
	if (HasAuthority() == false)
	{
		SpawnEffect();
	}
	Super::Destroyed();
}

void AGoblinProjectile::SpawnEffect()
{
	if(bHit == true)
		return;
	if (ImpactSound)
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	if (ImpactEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if(IsValid(LoopingSoundComponent))
		LoopingSoundComponent->Stop();
}

void AGoblinProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//자기 자신이 맞는 경우는 제외한다.

	/*
		강좌에서는 IsValid를 확인한다. 보아하니 클라이언트에서는 데이터가 없나보다. 그런데 의문인게 이런식으로 걸러내면, 
		아래에서 HasAuthority를 확인하는게 의미 없을거 같은데?
	*/
	if(DamageEffectSpecHandle.Data.IsValid() == false)
		return;

	AActor* Causer = DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser();
	if ( OtherActor == Causer)
		return;
	
	bool Ret = UGoblinBlueprintFunctionLibrary::IsNotFriend(OtherActor, Causer);
	if (Ret == false)
		return;

	SpawnEffect();

	//파괴는 서버에서 진행이 되어야 한다.
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		
		ABaseCharacter* BC = Cast<ABaseCharacter>(Causer);
		ABaseCharacter* Target = Cast<ABaseCharacter>(OtherActor);
		if (Target)
		{
			BC->DelegaeHitResult.Broadcast(SweepResult);
		}

		Destroy();
	}
	else
	{
		bHit = true;
	}
}



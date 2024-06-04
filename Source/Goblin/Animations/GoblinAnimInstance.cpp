// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinAnimInstance.h"
#include "Characters/BaseCharacter.h"
#include "Math/UnrealMathUtility.h"

UGoblinAnimInstance::UGoblinAnimInstance()
{

}

void UGoblinAnimInstance::NativeInitializeAnimation()
{
	Owner = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UGoblinAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if(Owner == nullptr)
		return;
	if(Owner->bDead == false)
		Speed = Owner->GetVelocity().Length();
	else
		Speed = 0;

	if (Owner->bDead == true)
		return;

	MoveForward = Owner->MoveForward;
	MoveRight = Owner->MoveRight;
	TargetLowerRotation = MoveRight * 30;
	TargetLowerRotation *= MoveForward;
	AnimationState = Owner->AnimationState;
	MoveState = Owner->CharacterMoveState;
	CurrentLowerRotation = FMath::FInterpTo<float>(CurrentLowerRotation, TargetLowerRotation, DeltaSeconds, 0.1f);;

	//다른 사람 컴퓨터에서만 내 플레이어가 고개를 들고 하는게 보인다.
	//if(Owner->GetController() != GetWorld()->GetFirstPlayerController())
	//{
	//	ControllerPitch = Owner->ControllerPitch;
	//}
	if(Owner->CharacterClass == ECharacterClass::Player)
		ControllerPitch = Owner->ControllerPitch;

	if (Owner->HeldAbilityItem)
	{
		HasFlashlight = true;
	}
	else
	{
		HasFlashlight = false;
	}

	//if(Owner->GetController() != nullptr)
	//{ 
	//	ControllerPitch = Owner->GetController()->GetControlRotation().Pitch;
	//}
	//	//else if (Owner->HasAuthority() == false)
	//	//{
	//	//	ControllerPitch = Owner->GetController()->GetControlRotation().Pitch;
	//	//}

	//	////호스트
	//	//if (Owner->GetController() != GetWorld()->GetFirstPlayerController())
	//	//{
	//	//	ControllerPitch = Owner->GetController()->GetControlRotation().Pitch;
	//	//}
	//	////클라
	//	//else if (Owner->GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
	//	//{
	//	//	
	//	//}
	//}


}
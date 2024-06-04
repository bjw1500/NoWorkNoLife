// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinWidgetController.h"

void UGoblinWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UGoblinWidgetController::BroadCastInitalValues()
{
}

void UGoblinWidgetController::BindCallbacksToDependencies()
{
}

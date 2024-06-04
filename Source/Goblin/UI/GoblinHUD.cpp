// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GoblinHUD.h"
#include "GoblinUserWidget.h"
#include "InGameWidgetController.h"
#include "GoblinWidgetController.h"

void AGoblinHUD::Init(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	/*
		초기화는 컨트롤되는 Character에서 진행이 된다.
	*/
	checkf(InGameUI_WidgetClass, TEXT("ARPG HUD ERROR! WidgetClass 확인"));

	const FWidgetControllerParams WidgetControllerParam(PC, PS, ASC, AS);
	InGameWidgetController = GetInGameWidgetController(WidgetControllerParam);

	InGameUI_Widget = CreateWidget<UGoblinUserWidget>(GetWorld(), InGameUI_WidgetClass);
	InGameUI_Widget->SetWidgetController(InGameWidgetController);
	InGameWidgetController->BroadCastInitalValues();
	InGameUI_Widget->AddToViewport();

}

UInGameWidgetController* AGoblinHUD::GetInGameWidgetController(const FWidgetControllerParams& WCParams)
{
	if (InGameWidgetController == nullptr)
	{
		InGameWidgetController = NewObject<UInGameWidgetController>(this, InGameWidgetControllerClass);
		InGameWidgetController->SetWidgetControllerParams(WCParams);
		InGameWidgetController->BindCallbacksToDependencies();
		return InGameWidgetController;
	}


	return InGameWidgetController;
}

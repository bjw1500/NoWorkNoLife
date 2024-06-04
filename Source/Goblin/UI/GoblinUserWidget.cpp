// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinUserWidget.h"

void UGoblinUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;

	WidgetControllerSet();
}

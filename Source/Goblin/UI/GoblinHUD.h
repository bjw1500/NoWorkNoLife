// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GoblinHUD.generated.h"

/**
 
	UI Manager 용도로 사용할 예정.

	실행 플로우
	게임 시작 -> GameMode에 의해 HUD 생성 -> 플레이어 컨트롤러가 캐릭터 조종시 HUD::Init 실행 

 */

class UGoblinUserWidget;
class UInGameWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
struct FWidgetControllerParams;

UCLASS()
class GOBLIN_API AGoblinHUD : public AHUD
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Init(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	UInGameWidgetController* GetInGameWidgetController(const FWidgetControllerParams& WCParams);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UGoblinUserWidget> InGameUI_Widget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGoblinUserWidget> InGameUI_WidgetClass;
	
	/*InGameController*/
	UPROPERTY()
	TObjectPtr<UInGameWidgetController> InGameWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInGameWidgetController> InGameWidgetControllerClass;
};

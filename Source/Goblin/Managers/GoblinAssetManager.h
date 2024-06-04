// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GoblinAssetManager.generated.h"

/**
* 언리얼 안에서 자체적으로 지원하는 매니저.
	AssetManager는 엔진 실행 초기에 생성되고, 실행되는 객체이다.
	이름 그대로 Asset을 관리하는 매니저인데 활용법이 어렵고, 문서가 많이 없어서,
	[엔진 실행과 함께 초기에 실행되는 부분을 이용하는 것] 말고는 활용은 잘 안할 예정.

	추후 AssetManager를 교체할 필요가 있다면.
	DefaultEngine.ini 문서에서 설정 바꿔주기.
	AssetManagerClassName=/Script/Goblin.GoblinAssetManager
 */
UCLASS()
class GOBLIN_API UGoblinAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UGoblinAssetManager& Get();

	/*엔진 실행 초기에 실행되는 함수*/
	virtual void StartInitialLoading() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectInterface.generated.h"

// This class does not need to be modified.

class AGoblinPlayerController;

UINTERFACE(MinimalAPI)
class UObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GOBLIN_API IObjectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HighlightObject(AGoblinPlayerController* Player = nullptr) = 0;
	virtual void UnHighlightObject(AGoblinPlayerController* Player = nullptr) = 0;
};

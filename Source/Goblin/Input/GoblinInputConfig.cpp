// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinInputConfig.h"
#include "InputAction.h"

const UInputAction* UGoblinInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FGoblinInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}

	}
	return nullptr;
}

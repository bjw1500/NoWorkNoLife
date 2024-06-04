// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}

TArray<FCharacterClassDefaultInfo> UCharacterClassInfo::GetClassDefaultInfoByRarity(ERarity Rariry)
{
	TArray<FCharacterClassDefaultInfo> TempArray;
	CharacterClassInformation.GenerateValueArray(TempArray);

	TArray<FCharacterClassDefaultInfo> Result;

	for (FCharacterClassDefaultInfo Info : TempArray)
	{
		if(Info.CharacterClass == ECharacterClass::Player)
			continue;

		if(Info.Rarity == Rariry)
			Result.Add(Info);
	}
	
	return Result;
}

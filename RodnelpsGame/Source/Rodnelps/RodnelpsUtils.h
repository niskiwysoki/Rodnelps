// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RodnelpsUtils.generated.h"

class ACard;
class AToken;

USTRUCT()
struct FCardArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<ACard*> m_Cards;
};

USTRUCT()
struct FTokenArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AToken*> m_Tokens;
};
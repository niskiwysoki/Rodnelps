// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerBoardSpace.h"
#include "Token.h"
#include "RodnelpsPlayerState.generated.h"


UCLASS()
class RODNELPS_API ARodnelpsPlayerState : public APlayerState//, public IOwnershipInterface
{
	GENERATED_BODY()

public:
	ARodnelpsPlayerState();

	void setPlayerBoard(APlayerBoardSpace* playerBoard);
	
	APlayerBoardSpace* getPlayerBoard();

	void addToken(AToken* token);

	int32 getTokenNum();

private:
	TArray<TArray<AToken*>> m_TokenStacksArray;

	ETokenColor m_FirstTokenTakenColor;
	ETokenColor m_SecondTokenTakenColor;
	TArray<bool> m_ReservedCards;

	APlayerBoardSpace* m_PlayerBoard;
};

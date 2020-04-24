// Fill out your copyright notice in the Description page of Project Settings.


#include "RodnelpsPlayerState.h"

ARodnelpsPlayerState::ARodnelpsPlayerState()
{
	m_FirstTokenTakenColor = ETokenColor::MAX_COLOURS;
	m_SecondTokenTakenColor = ETokenColor::MAX_COLOURS;
	m_ReservedCards = { 0,0,0 };
	for (size_t i = 0; i < (int)ETokenColor::MAX_COLOURS; i++)
	{
		TArray<AToken*> tokenArray;
		m_TokenStacksArray.Push(tokenArray);
	}
	
}

void ARodnelpsPlayerState::setPlayerBoard(APlayerBoardSpace* playerBoard)
{
	m_PlayerBoard = playerBoard;
}

APlayerBoardSpace* ARodnelpsPlayerState::getPlayerBoard()
{
	return m_PlayerBoard;
}

void ARodnelpsPlayerState::addToken(AToken* token)
{
	m_TokenStacksArray[int32(token->getColor())].Push(token);

}

int32 ARodnelpsPlayerState::getTokenNum()
{
	int32 tokenSum = 0;
	for (const auto& tokenStack : m_TokenStacksArray) {
		tokenSum += tokenStack.Num();
	}
	return tokenSum;
}


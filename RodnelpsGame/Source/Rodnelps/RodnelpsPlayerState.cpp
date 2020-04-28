// Fill out your copyright notice in the Description page of Project Settings.


#include "RodnelpsPlayerState.h"
#include "RodnelpsGameState.h"
#include "InterpolationManager.h"
#include "GameElementsGenerator.h"
#include "Engine/TargetPoint.h"

ARodnelpsPlayerState::ARodnelpsPlayerState()
{
	m_FirstTokenTakenColor = ETokenColor::MAX_COLOURS;
	m_SecondTokenTakenColor = ETokenColor::MAX_COLOURS;
	m_AreTokensDrawn = 0;
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
	// if 3rd token is the same as first or second
	if((token->getColor() == m_FirstTokenTakenColor || m_SecondTokenTakenColor == token->getColor()) && m_SecondTokenTakenColor != ETokenColor::MAX_COLOURS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Taken other token than first or second"));
		return;
	}

	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	int32 tokenColorIdnex = int32(token->getColor());
	token->setOwner(this);
	m_TokenStacksArray[tokenColorIdnex].Push(token);
	gamestate->getGameElementGenerator()->removeToken(token);

	//interp
	gamestate->GetInterpolationManager()->setDesiredLocation(token, token->GetActorLocation() + FVector(0.f, 0.f, 400.f), 0);
	ATargetPoint* point = getPlayerBoard()->getTokenTargetPoints()[tokenColorIdnex];
	int32 numOfTokenInStack = m_TokenStacksArray[tokenColorIdnex].Num();
	gamestate->GetInterpolationManager()->setDesiredLocation(token, point->GetActorLocation() + FVector(0.f, 0.f, numOfTokenInStack * 20.f), 0.f);

	if (m_FirstTokenTakenColor == ETokenColor::MAX_COLOURS)
	{
		m_FirstTokenTakenColor = token->getColor();
	}
	else
	{
		if (m_SecondTokenTakenColor == ETokenColor::MAX_COLOURS)
		{
			m_SecondTokenTakenColor = token->getColor();
			if (m_FirstTokenTakenColor == m_SecondTokenTakenColor)
			{
				m_AreTokensDrawn = true;
				resetStatusAndEndTurn(gamestate);
			}
		}
		else
		{
			m_AreTokensDrawn = true;
			resetStatusAndEndTurn(gamestate);
		}
	}
}

void ARodnelpsPlayerState::removeToken(AToken* token)
{
	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	gamestate->getGameElementGenerator()->addToken(token);
	m_TokenStacksArray[int32(token->getColor())].Pop(token);
	resetStatusAndEndTurn(gamestate);
}

void ARodnelpsPlayerState::resetStatusAndEndTurn(ARodnelpsGameState* gamestate)
{
	if (this->getTokenNum() <= 10)
	{
		m_FirstTokenTakenColor = ETokenColor::MAX_COLOURS;
		m_SecondTokenTakenColor = ETokenColor::MAX_COLOURS;
		m_AreTokensDrawn = 0;
		gamestate->endTurn();
	}
}

int32 ARodnelpsPlayerState::getTokenNum()
{
	int32 tokenSum = 0;
	for (const auto& tokenStack : m_TokenStacksArray) {
		tokenSum += tokenStack.Num();
	}
	return tokenSum;
}

bool ARodnelpsPlayerState::areTokensDrawn()
{
	return m_AreTokensDrawn;
}

bool ARodnelpsPlayerState::isTaken_Implementation()
{
	return true;
}



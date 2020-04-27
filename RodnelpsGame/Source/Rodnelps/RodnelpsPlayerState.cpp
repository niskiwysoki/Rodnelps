// Fill out your copyright notice in the Description page of Project Settings.


#include "RodnelpsPlayerState.h"
#include "RodnelpsGameState.h"
#include "InterpolationManager.h"
#include "Engine/TargetPoint.h"

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
	int32 tokenColorIdnex = int32(token->getColor());
	token->setOwner(this);
	m_TokenStacksArray[tokenColorIdnex].Push(token);
	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	gamestate->GetInterpolationManager()->setDesiredLocation(token, token->GetActorLocation() + FVector(0.f, 0.f, 400.f), 0);
	ATargetPoint* point = getPlayerBoard()->getTokenTargetPoints()[tokenColorIdnex];
	int32 numOfTokenInStack = m_TokenStacksArray[tokenColorIdnex].Num();
	gamestate->GetInterpolationManager()->setDesiredLocation(token, point->GetActorLocation() + FVector(0.f, 0.f, numOfTokenInStack * 20.f), 0.f);


}

int32 ARodnelpsPlayerState::getTokenNum()
{
	int32 tokenSum = 0;
	for (const auto& tokenStack : m_TokenStacksArray) {
		tokenSum += tokenStack.Num();
	}
	return tokenSum;
}

bool ARodnelpsPlayerState::isTaken_Implementation()
{
	return true;
}



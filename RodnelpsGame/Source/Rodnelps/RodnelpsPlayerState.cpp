// Fill out your copyright notice in the Description page of Project Settings.


#include "RodnelpsPlayerState.h"
#include "RodnelpsGameState.h"
#include "InterpolationManager.h"
#include "GameElementsGenerator.h"
#include "Engine/TargetPoint.h"
#include "Card.h"

ARodnelpsPlayerState::ARodnelpsPlayerState()
{
	m_FirstTokenTakenColor = ETokenColor::MAX_COLOURS;
	m_SecondTokenTakenColor = ETokenColor::MAX_COLOURS;
	m_AreTokensDrawn = 0;
	for (size_t i = 0; i < (int)ETokenColor::GOLD; i++)
	{
		TArray<AToken*> tokenArray;
		TArray<ACard*> cardArray;
		m_CardStacksArray.Push(cardArray);
		m_TokenStacksArray.Push(tokenArray);
	}
	//tokens have one color more
	TArray<AToken*> goldTokenArray;
	m_TokenStacksArray.Push(goldTokenArray);
}

void ARodnelpsPlayerState::setPlayerBoard(APlayerBoardSpace* playerBoard)
{
	m_PlayerBoard = playerBoard;
}

APlayerBoardSpace* ARodnelpsPlayerState::getPlayerBoard()
{
	return m_PlayerBoard;
}

void ARodnelpsPlayerState::addStandardToken(AToken* token)
{
	// if 3rd token is the same as first or second
	if((token->getColor() == m_FirstTokenTakenColor || m_SecondTokenTakenColor == token->getColor()) && m_SecondTokenTakenColor != ETokenColor::MAX_COLOURS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Taken other token than first or second"));
		return;
	}

	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	
	if (m_FirstTokenTakenColor == ETokenColor::MAX_COLOURS)
	{
		m_FirstTokenTakenColor = token->getColor();
	}
	else
	{
		if (m_SecondTokenTakenColor == ETokenColor::MAX_COLOURS)
		{
			if ((token->getColor() == m_FirstTokenTakenColor) && (gamestate->getGameElementGenerator()->getStackSize(token) < 3))
			{
				UE_LOG(LogTemp, Warning, TEXT("You can't take 2 tokens from stack with size 3 or less"));
				return;
			}
			else
			{
				m_SecondTokenTakenColor = token->getColor();
				if (m_FirstTokenTakenColor == m_SecondTokenTakenColor)
				{
					m_AreTokensDrawn = true;
					if (getTokenNum() <= 10)
					{
						resetTokenStatusAndEndTurn(gamestate);
					}
				}
			}
		}
		else
		{
			m_AreTokensDrawn = true;
			if (getTokenNum() <= 10)
			{
				resetTokenStatusAndEndTurn(gamestate);
			}
		}
	}

	addToken(token);
}

void ARodnelpsPlayerState::addToken(AToken* token)
{
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
}

void ARodnelpsPlayerState::removeToken(AToken* token)
{
	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	gamestate->getGameElementGenerator()->addToken(token);
	m_TokenStacksArray[int32(token->getColor())].Pop(token);
}

void ARodnelpsPlayerState::resetTokenStatusAndEndTurn(ARodnelpsGameState* gamestate)
{
		m_FirstTokenTakenColor = ETokenColor::MAX_COLOURS;
		m_SecondTokenTakenColor = ETokenColor::MAX_COLOURS;
		m_AreTokensDrawn = 0;
		gamestate->endTurn();
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

bool ARodnelpsPlayerState::areCardRequirementsFulfilled(ACard* card)
{
	int32 goldTokenRequired = 0;
	if (0 < card->getCardInfo()->ReqWhite - (m_CardStacksArray[0].Num() + m_TokenStacksArray[0].Num()))
		goldTokenRequired += card->getCardInfo()->ReqWhite - (m_CardStacksArray[0].Num() + m_TokenStacksArray[0].Num());
	if (0 < card->getCardInfo()->ReqBlue - (m_CardStacksArray[1].Num() + m_TokenStacksArray[1].Num()))
		goldTokenRequired += card->getCardInfo()->ReqBlue - (m_CardStacksArray[1].Num() + m_TokenStacksArray[1].Num());
	if (0 < card->getCardInfo()->ReqGreen - (m_CardStacksArray[2].Num() + m_TokenStacksArray[2].Num()))
		goldTokenRequired += card->getCardInfo()->ReqGreen - (m_CardStacksArray[2].Num() + m_TokenStacksArray[2].Num());
	if (0 < card->getCardInfo()->ReqRed - (m_CardStacksArray[3].Num() + m_TokenStacksArray[3].Num()))
		goldTokenRequired += card->getCardInfo()->ReqRed - (m_CardStacksArray[3].Num() + m_TokenStacksArray[3].Num());
	if (0 < card->getCardInfo()->ReqBlack - (m_CardStacksArray[4].Num() + m_TokenStacksArray[4].Num()))
		goldTokenRequired += card->getCardInfo()->ReqBlack - (m_CardStacksArray[4].Num() + m_TokenStacksArray[4].Num());
	
	return (m_TokenStacksArray[5].Num() >= goldTokenRequired);
}

void ARodnelpsPlayerState::addCard(ACard* card)
{
	card->setAsTaken();
	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	gamestate->GetInterpolationManager()->setDesiredLocation(card, card->GetActorLocation() + FVector(0.f, 0.f, 500.f), 0.f);

	int32 colorIndex = int32(card->getCardInfo()->CardColor);
	int32 cardsInColor = m_CardStacksArray[colorIndex].Num();
	ATargetPoint* desiredPoint = getPlayerBoard()->getCardTargetPoints()[colorIndex];
	gamestate->GetInterpolationManager()->setDesiredLocation(card, desiredPoint->GetActorLocation() + FVector(-50.f * cardsInColor, 0.f, 10.f * cardsInColor), 0.f);
	
	payForCard(card);
	m_CardStacksArray[colorIndex].Push(card);
	if (card->isReserved())
	{
		card->setAsNotReserved();
		m_ReservedCardArray.Remove(card);
	}
	else
	{
		gamestate->getGameElementGenerator()->placeNewCard(card);
	}
	gamestate->endTurn();
}

void ARodnelpsPlayerState::payForCard(ACard* card)
{
	int32 numOfTokensToDiscard = card->getCardInfo()->ReqWhite - m_CardStacksArray[0].Num();
	payTokenStackForCard(numOfTokensToDiscard, 0);
	numOfTokensToDiscard = card->getCardInfo()->ReqBlue - m_CardStacksArray[1].Num();
	payTokenStackForCard(numOfTokensToDiscard, 1);
	numOfTokensToDiscard = card->getCardInfo()->ReqGreen - m_CardStacksArray[2].Num();
	payTokenStackForCard(numOfTokensToDiscard, 2);
	numOfTokensToDiscard = card->getCardInfo()->ReqRed - m_CardStacksArray[3].Num();
	payTokenStackForCard(numOfTokensToDiscard, 3);
	numOfTokensToDiscard = card->getCardInfo()->ReqBlack - m_CardStacksArray[4].Num();
	payTokenStackForCard(numOfTokensToDiscard, 4);
}

void ARodnelpsPlayerState::reserveCard(ACard* card)
{
	int32 reservedCardArraySize = m_ReservedCardArray.Num();
	if (reservedCardArraySize >= 3)
	{
		return;	
	}

	card->setAsReserved();
	card->setAsTaken();
	m_ReservedCardArray.Push(card);

	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	gamestate->GetInterpolationManager()->setDesiredLocation(card, card->GetActorLocation() + FVector(0.f, 0.f, 500.f), 0.f);
	ATargetPoint* desiredPoint = getPlayerBoard()->getReservedCardsTargetPoints()[reservedCardArraySize];
	gamestate->GetInterpolationManager()->setDesiredLocation(card, desiredPoint->GetActorLocation(),0.f);

	AToken* goldToken = gamestate->getGameElementGenerator()->getGoldTokenStack().Last();
	addToken(goldToken);

	gamestate->getGameElementGenerator()->placeNewCard(card);
	gamestate->endTurn();
}

bool ARodnelpsPlayerState::isTaken_Implementation()
{
	return true;
}

void ARodnelpsPlayerState::payTokenStackForCard(int32 tokensNum, int32 stackColorIndex)
{
	while (tokensNum)
	{
		if (m_TokenStacksArray[stackColorIndex].Num() == 0)
		{
			removeToken(m_TokenStacksArray[int32(ETokenColor::GOLD)].Last());
		}
		removeToken(m_TokenStacksArray[stackColorIndex].Last());
		tokensNum--;
	}
}


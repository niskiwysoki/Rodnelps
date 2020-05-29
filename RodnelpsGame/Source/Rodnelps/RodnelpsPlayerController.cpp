// Fill out your copyright notice in the Description page of Project Settings.


#include "RodnelpsPlayerController.h"
#include "RodnelpsPlayerState.h"

ARodnelpsPlayerController::ARodnelpsPlayerController()
{

}

void ARodnelpsPlayerController::setPlayerScore(ARodnelpsPlayerState* playerstate)
{
	m_PlayerScore.NumbersOfTokensArray.Empty();
	m_PlayerScore.NumbersOfCardsArray.Empty();
	m_PlayerScore.VictoryPoints = 0;

	for (const auto& TokenStack : playerstate->getTokenStacksArray())
	{
		m_PlayerScore.NumbersOfTokensArray.Add(TokenStack.m_Tokens.Num());
	}
	for (const auto& CardStack : playerstate->getCardStacksArray())
	{
		m_PlayerScore.NumbersOfCardsArray.Add(CardStack.m_Cards.Num());
	}
	m_PlayerScore.VictoryPoints = playerstate->getVictoryPoints();

}

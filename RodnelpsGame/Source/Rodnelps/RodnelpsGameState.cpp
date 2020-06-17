// Fill out your copyright notice in the Description page of Project Settings.

#include "RodnelpsGameState.h"
#include "RodnelpsPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBoardSpace.h"
#include "GameElementsGenerator.h"
#include "RodnelpsGameMode.h"
#include "Net/UnrealNetwork.h"

ARodnelpsGameState::ARodnelpsGameState()
{
	m_ActivePlayer = nullptr;
	m_NumberOfPlayers = 1;
}

ARodnelpsPlayerState* ARodnelpsGameState::getActivePlayer()
{
	return m_ActivePlayer;
}

void ARodnelpsGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARodnelpsGameState, m_ActivePlayer);
}

void ARodnelpsGameState::setInterpolationManager(AInterpolationManager* interpolManager)
{
	m_InterpolManager = interpolManager;
}

AInterpolationManager* ARodnelpsGameState::GetInterpolationManager()
{
	return m_InterpolManager;
}

AGameElementsGenerator* ARodnelpsGameState::getGameElementGenerator()
{
	return m_GameElementsGenerator;
}

void ARodnelpsGameState::setGameElementGenerator(AGameElementsGenerator* gameElementsGenerator)
{
	m_GameElementsGenerator = gameElementsGenerator;

	tryGeneratePieces();
}

void ARodnelpsGameState::tryGeneratePieces()
{
	if (HasAuthority())
	{
		if (m_NumberOfPlayers > 0)
		{
			if (m_GameElementsGenerator)
			{
				m_GameElementsGenerator->generateGamePieces(m_NumberOfPlayers);
			}
		}
	}
}

void ARodnelpsGameState::Server_setActivePlayer(ARodnelpsPlayerState* player)
{
	check(HasAuthority());

	if (HasAuthority())
	{
		m_ActivePlayer = player;
	}
}

void ARodnelpsGameState::setNumberOfPlayers(int32 val)
{
	m_NumberOfPlayers = val;

	tryGeneratePieces();
}

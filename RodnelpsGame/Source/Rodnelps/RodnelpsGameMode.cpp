// Fill out your copyright notice in the Description page of Project Settings.

#include "RodnelpsGameMode.h"
#include "PlayerBoardSpace.h"
#include "RodnelpsPlayerState.h"
#include "RodnelpsGameState.h"
#include "GameElementsGenerator.h"

void ARodnelpsGameMode::BeginPlay()
{
	Super::BeginPlay();
	s_IdCounter = 0;
}

void ARodnelpsGameMode::RegisterPlayerState(ARodnelpsPlayerState* playerState)
{
	const int32 id = s_IdCounter++;

	playerState->setPlayerId(id);
	playerState->setPlayerBoard(GetPlayerBoard(id));

	m_PlayersArray.Add(playerState);

	UE_LOG(LogTemp, Error, TEXT("New Player - %d"), m_PlayersArray.Num());
}

void ARodnelpsGameMode::registerPlayerBoard(APlayerBoardSpace* board)
{
	m_BoardArray.Add(board);

	// When player registered first, now we need to set him proper board during board registration
	for (auto playerState : m_PlayersArray)
	{
		if (playerState->getPlayerId() == board->getPlayerId())
		{
			playerState->setPlayerBoard(board);
			break;
		}
	}
}

APlayerBoardSpace* ARodnelpsGameMode::GetPlayerBoard(int32 id)
{
	for (const auto board : m_BoardArray)
	{
		if (board->getPlayerId() == id)
		{
			return board;
		}
	}

	return nullptr;
}

void ARodnelpsGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	if (m_PlayersArray.Num() > 0)
		setActivePlayer(m_PlayersArray[0]);
	else
		UE_LOG(LogTemp, Error, TEXT("No Players!"));
}

void ARodnelpsGameMode::setActivePlayer(ARodnelpsPlayerState* player)
{
	if (m_ActivePlayer != player)
	{
		m_ActivePlayer = player;

		if (ARodnelpsGameState* state = GetWorld()->GetGameState<ARodnelpsGameState>())
		{
			state->Server_setActivePlayer(player);
		}
	}
}

void ARodnelpsGameMode::setNextActivePlayer()
{
	for (int32 i = 0; i < m_PlayersArray.Num(); i++)
	{
		if (m_PlayersArray[i] == m_ActivePlayer)
		{
			setActivePlayer(m_PlayersArray[(i + 1) % m_PlayersArray.Num()]);
			break;
		}
	}
}

void ARodnelpsGameMode::endTurn()
{
	if (m_ActivePlayer->isTakingTraders())
	{
		m_ActivePlayer->setIsTakingTraders(false);
		//TODO change active player
		UE_LOG(LogTemp, Warning, TEXT("NewTurn"))
			return;
	}

	if (m_ActivePlayer->isTraderPosibbleToGet())
	{
		TArray<ATraderCard*> accessibleTradersArray;
		if (ARodnelpsGameState* state = GetWorld()->GetGameState<ARodnelpsGameState>())
		{
			for (const auto& trader : state->getGameElementGenerator()->getTraderArray())
			{
				if (m_ActivePlayer->isMeetsTraderRequirements(trader))
				{
					accessibleTradersArray.Push(trader);
				}
			}
		}

		if (accessibleTradersArray.Num() == 1)
		{
			m_ActivePlayer->transferTrader(accessibleTradersArray[0]);

		}
		else
		{
			m_ActivePlayer->setIsTakingTraders(true);
			UE_LOG(LogTemp, Warning, TEXT("Take one of traders"))
		}
	}
	else
	{
		//TODO change active player
		UE_LOG(LogTemp, Warning, TEXT("NewTurn"))
		setNextActivePlayer();		
	}
}

int32 ARodnelpsGameMode::s_IdCounter = 0;

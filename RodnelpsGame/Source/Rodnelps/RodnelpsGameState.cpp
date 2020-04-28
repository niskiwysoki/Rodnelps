// Fill out your copyright notice in the Description page of Project Settings.


#include "RodnelpsGameState.h"
#include "RodnelpsPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBoardSpace.h"


ARodnelpsGameState::ARodnelpsGameState()
{
	m_ActivePlayer = nullptr;
}

ARodnelpsPlayerState* ARodnelpsGameState::getActivePlayer()
{
	return m_ActivePlayer;
}

void ARodnelpsGameState::addPlayer(APlayerBoardSpace* playerBoard)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), playerBoard->getPlayerId());
	if (playerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("playerConetroller nullptr"))
	}
	else
	{
		ARodnelpsPlayerState* playerState = playerController->GetPlayerState<ARodnelpsPlayerState>();

		if (playerState)
		{
			playerState->setPlayerBoard(playerBoard);
			m_PlayersArray.Push(playerState);

			//TODO change this later!
			if (playerBoard->getPlayerId() == 0)
			{
				m_ActivePlayer = playerState;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No such playerState"))
		}
	}
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
}

void ARodnelpsGameState::endTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("NewTurn"))
}

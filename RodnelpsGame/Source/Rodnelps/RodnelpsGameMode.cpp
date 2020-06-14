// Fill out your copyright notice in the Description page of Project Settings.

#include "RodnelpsGameMode.h"
#include "PlayerBoardSpace.h"
#include "RodnelpsPlayerState.h"
#include "RodnelpsGameState.h"
#include "GameElementsGenerator.h"
#include "RodnelpsPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DefaultValueHelper.h"


void ARodnelpsGameMode::BeginPlay()
{
	Super::BeginPlay();
	m_Time = 0;
	m_IsLastRound = false;
	PrimaryActorTick.bCanEverTick = true;
	FDefaultValueHelper::ParseInt(UGameplayStatics::ParseOption(OptionsString, "NumberOfPlayers"), m_PlayerNumber);
	if (ARodnelpsGameState* gameState = GetWorld()->GetGameState<ARodnelpsGameState>())
	{
		gameState->setNumberOfPlayers(m_PlayerNumber);
	}	
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

void ARodnelpsGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	GetWorld()->GetTimerManager().SetTimer(EndGameTimer, this, &ARodnelpsGameMode::OnExitMap, 3, false);
}

void ARodnelpsGameMode::OnExitMap()
{
	for (auto player : m_PlayersArray)
	{
		if (APlayerController* playerController = Cast<APlayerController>(player->GetOwner()))
		{
			playerController->ConsoleCommand("servertravel/Game/Maps/Test/Lobby");
		}
	}
}


void ARodnelpsGameMode::setActivePlayer(ARodnelpsPlayerState* player)
{
	if (m_ActivePlayer != player)
	{
		m_ActivePlayer = player;

		if (ARodnelpsGameState* gameState = GetWorld()->GetGameState<ARodnelpsGameState>())
		{
			gameState->Server_setActivePlayer(player);
		}
	}
}

void ARodnelpsGameMode::setNextActivePlayer()
{
	int32 indexOfLastPlayer = m_PlayersArray.Num() -1;
	for (int32 i = 0; i <= indexOfLastPlayer; i++)
	{
		if (m_PlayersArray[i] == m_ActivePlayer)
		{
			if (m_ActivePlayer->getVictoryPoints() >= 15)
				m_IsLastRound = true;

			if (m_IsLastRound && i == indexOfLastPlayer)
			{
				gameSummary();
			}
			else
			{
				setActivePlayer(m_PlayersArray[(i + 1) % m_PlayersArray.Num()]);
				UE_LOG(LogTemp, Warning, TEXT("NewTurn"))
				m_ActivePlayer->broadcast_SendGuideMessage("Your Turn");
				m_ActivePlayer->broadcast_showMessageOnCenterOfScreen("Your turn", 0.7);
				break;
			}
		}
	}
}

void ARodnelpsGameMode::endTurn()
{
	if (m_ActivePlayer->isTakingTraders())
	{
		m_ActivePlayer->setIsTakingTraders(false);
		UpdateScoreTable();
		setNextActivePlayer();
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
			m_ActivePlayer->sendGuideMessage("Take one of traders");
		}
	}
	else
	{
		UpdateScoreTable();
		setNextActivePlayer();	
	}
}

void ARodnelpsGameMode::gameSummary()
{
	ARodnelpsPlayerState* bestPlayer = m_PlayersArray[0];
	int32 indexOfLastPlayer = m_PlayersArray.Num() - 1;
	for (int32 i = 1; i <= indexOfLastPlayer; i++)
	{
		if (bestPlayer->getVictoryPoints() > m_PlayersArray[i]->getVictoryPoints())
			m_PlayersArray[i]->broadcast_showMessageOnCenterOfScreen("You lost", 5.f);
		else if (bestPlayer->getVictoryPoints() == m_PlayersArray[i]->getVictoryPoints())
		{
			if (getPlayerCardSum(bestPlayer) < getPlayerCardSum(m_PlayersArray[i]))
			{
				m_PlayersArray[i]->broadcast_showMessageOnCenterOfScreen("You lost", 5.f);
			}
			else
			{
				bestPlayer->broadcast_showMessageOnCenterOfScreen("You lost", 5.f);
				bestPlayer = m_PlayersArray[i];
			}
		}
		else
		{
			bestPlayer->broadcast_showMessageOnCenterOfScreen("You lost", 5.f);
			bestPlayer = m_PlayersArray[i];
		}
	}
	bestPlayer->broadcast_showMessageOnCenterOfScreen("Congratulations, you didn't lose", 5.f);
	
	EndMatch();
}

void ARodnelpsGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	s_IdCounter = 0;
}



int32 ARodnelpsGameMode::getPlayerCardSum(ARodnelpsPlayerState* player)
{
	int32 cardNum = 0;
	for (const auto& cardStack : player->getCardStacksArray())
		cardNum += cardStack.m_Cards.Num();
	return cardNum;
}

int32 ARodnelpsGameMode::s_IdCounter = 0;

void ARodnelpsGameMode::UpdateScoreTable()
{
	if(ARodnelpsPlayerController* controller = Cast<ARodnelpsPlayerController>(m_ActivePlayer->GetOwner()))
	{
		controller->setPlayerScore(m_ActivePlayer);
	}
	UpdateScoreTableBP();
}

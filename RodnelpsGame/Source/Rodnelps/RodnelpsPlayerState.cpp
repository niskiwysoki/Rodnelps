// Fill out your copyright notice in the Description page of Project Settings.


#include "RodnelpsPlayerState.h"
#include "RodnelpsGameState.h"
#include "InterpolationManager.h"
#include "GameElementsGenerator.h"
#include "Engine/TargetPoint.h"
#include "Card.h"
#include "TraderCard.h"
#include "RodnelpsGameMode.h"
#include "Net/UnrealNetwork.h"
#include "RodnelpsPlayerController.h"
#include "Kismet/KismetStringLibrary.h"

ARodnelpsPlayerState::ARodnelpsPlayerState()
{

	m_FirstTokenTakenColor = ETokenColor::MAX_COLOURS;
	m_SecondTokenTakenColor = ETokenColor::MAX_COLOURS;
	m_AreTokensDrawn = false;
	m_isTakingTokens = false;
	m_isTakingTrader = false;
	m_VictoryPoints = 0;

	for (size_t i = 0; i < (int)ETokenColor::GOLD; i++)
	{
		m_CardStacksArray.Push(FCardArray());
		m_TokenStacksArray.Push(FTokenArray());
	}
	//tokens have one color more
	m_TokenStacksArray.Push(FTokenArray());

	//3slots of reserved cards
	m_ReservedCardArray.Push(nullptr);
	m_ReservedCardArray.Push(nullptr);
	m_ReservedCardArray.Push(nullptr);

	m_PlayerId = 0;
}

void ARodnelpsPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (ARodnelpsGameMode* gameMode = GetWorld()->GetAuthGameMode<ARodnelpsGameMode>())
		{
			gameMode->RegisterPlayerState(this);
		}
	}
}


void ARodnelpsPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARodnelpsPlayerState, m_PlayerId);
	DOREPLIFETIME(ARodnelpsPlayerState, m_PlayerBoard);
	DOREPLIFETIME(ARodnelpsPlayerState, m_CardStacksArray);
	DOREPLIFETIME(ARodnelpsPlayerState, m_TokenStacksArray);
	DOREPLIFETIME(ARodnelpsPlayerState, m_ReservedCardArray);
	DOREPLIFETIME(ARodnelpsPlayerState, m_TraderCardArray);
	DOREPLIFETIME(ARodnelpsPlayerState, m_isTakingTrader);
	DOREPLIFETIME(ARodnelpsPlayerState, m_AreTokensDrawn);
	DOREPLIFETIME(ARodnelpsPlayerState, m_isTakingTokens);
}

void ARodnelpsPlayerState::setPlayerBoard(APlayerBoardSpace* playerBoard)
{
	check(HasAuthority());

	if (HasAuthority())
	{
		m_PlayerBoard = playerBoard;
	}
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
		sendGuideMessage("Taken other token than first or second");
		return;
	}

	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	bool bResetAndEndTurn = false;

	if (m_FirstTokenTakenColor == ETokenColor::MAX_COLOURS)
	{
		m_FirstTokenTakenColor = token->getColor();
		m_isTakingTokens = true;
	}
	else
	{
		if (m_SecondTokenTakenColor == ETokenColor::MAX_COLOURS)
		{
			if ((token->getColor() == m_FirstTokenTakenColor) && (gamestate->getGameElementGenerator()->getStackSize(token) < 3))
			{
				UE_LOG(LogTemp, Warning, TEXT("You can't take 2 tokens from stack with size 3 or less"));
				sendGuideMessage("You can't take 2 tokens from stack with size 3 or less");
				return;
			}
			else
			{
				m_SecondTokenTakenColor = token->getColor();
				if (m_FirstTokenTakenColor == m_SecondTokenTakenColor)
				{
					m_AreTokensDrawn = true;
					if (getTokenNum() < 10)
					{
						bResetAndEndTurn = true;
					}
					else
					{
						sendGuideMessage("Discard excess tokens");
						showMessageOnCenterOfScreen("Discard excess tokens", 0.5f);
					}
				}
			}
		}
		else
		{
			m_AreTokensDrawn = true;
			if (getTokenNum() < 10)
			{
				bResetAndEndTurn = true;
			}
			else
			{
				sendGuideMessage("Discard excess tokens");
				showMessageOnCenterOfScreen("Discard excess tokens", 0.5f);
			}
		}
	}
	addToken(token);
	if(!HasAuthority())
		m_TokenStacksArray[int32(token->getColor())].m_Tokens.Push(token);		// predicted on client
	
	if (!isPossibleToGetToken())
	{
		m_AreTokensDrawn = true;
		if (getTokenNum() <= 10)
		{
			sendGuideMessage(FString::FromInt(getTokenNum()) + TEXT(" : No tokens left"));
			bResetAndEndTurn = true;
		}
		else
		{
			sendGuideMessage("Discard excess tokens");
			showMessageOnCenterOfScreen("Discard excess tokens", 0.5f);
		}
	}

	if (bResetAndEndTurn)
	{
		resetTokenStatusAndEndTurn(gamestate);
	}

}

bool ARodnelpsPlayerState::addToken_Validate(AToken* token)
{
	return true;
}

void ARodnelpsPlayerState::addToken_Implementation(AToken* token)
{
	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	int32 tokenColorIdnex = int32(token->getColor());

	TArray<AToken*> tokenStackInColor = gamestate->getGameElementGenerator()->getTokenStack(token);
	AToken* tokenToAdd = tokenStackInColor.Last();
	tokenToAdd->setOwner(this);
	m_TokenStacksArray[tokenColorIdnex].m_Tokens.Push(tokenToAdd);
	gamestate->getGameElementGenerator()->removeToken(tokenToAdd);

	ATargetPoint* point = getPlayerBoard()->getTokenTargetPoints()[tokenColorIdnex];
	int32 numOfTokenInStack = m_TokenStacksArray[tokenColorIdnex].m_Tokens.Num();
	moveActorOnBoard(tokenToAdd, point->GetActorLocation() + FVector(0.f, 0.f, numOfTokenInStack * 20.f));
}

void ARodnelpsPlayerState::Server_removeToken_Implementation(AToken* token)
{
	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	AToken* tokenToRemove = m_TokenStacksArray[int32(token->getColor())].m_Tokens.Last();
	gamestate->getGameElementGenerator()->addToken(tokenToRemove);
	m_TokenStacksArray[int32(tokenToRemove->getColor())].m_Tokens.Pop(tokenToRemove);

}

bool ARodnelpsPlayerState::Server_removeToken_Validate(AToken* token)
{
	return true;
}

void ARodnelpsPlayerState::removeToken(AToken* token)
{
	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	AToken* tokenToRemove = m_TokenStacksArray[int32(token->getColor())].m_Tokens.Last();
	gamestate->getGameElementGenerator()->addToken(tokenToRemove);
	m_TokenStacksArray[int32(tokenToRemove->getColor())].m_Tokens.Pop(tokenToRemove);

	if(!HasAuthority())
		Server_removeToken(token);
}

void ARodnelpsPlayerState::resetTokenStatusAndEndTurn(ARodnelpsGameState* gamestate)
{
		m_FirstTokenTakenColor = ETokenColor::MAX_COLOURS;
		m_SecondTokenTakenColor = ETokenColor::MAX_COLOURS;
		m_AreTokensDrawn = false;
		m_isTakingTokens = false;
		endTurn();
}

int32 ARodnelpsPlayerState::getTokenNum()
{
	int32 tokenSum = 0;
	for (const auto& tokenStack : m_TokenStacksArray) {
		tokenSum += tokenStack.m_Tokens.Num();
	}
	return tokenSum;
}

bool ARodnelpsPlayerState::areTokensDrawn()
{
	return m_AreTokensDrawn;
}

bool ARodnelpsPlayerState::isTakingTokens()
{
	return m_isTakingTokens;
}

bool ARodnelpsPlayerState::isTakingTraders()
{
	return m_isTakingTrader;
}

void ARodnelpsPlayerState::setIsTakingTraders(bool status)
{
	m_isTakingTrader = status;
}

bool ARodnelpsPlayerState::areCardRequirementsFulfilled(ACard* card)
{
	int32 goldTokenRequired = 0;
	if (0 < card->getCardInfo()->ReqWhite - (m_CardStacksArray[0].m_Cards.Num() + m_TokenStacksArray[0].m_Tokens.Num()))
		goldTokenRequired += card->getCardInfo()->ReqWhite - (m_CardStacksArray[0].m_Cards.Num() + m_TokenStacksArray[0].m_Tokens.Num());
	if (0 < card->getCardInfo()->ReqBlue - (m_CardStacksArray[1].m_Cards.Num() + m_TokenStacksArray[1].m_Tokens.Num()))
		goldTokenRequired += card->getCardInfo()->ReqBlue - (m_CardStacksArray[1].m_Cards.Num() + m_TokenStacksArray[1].m_Tokens.Num());
	if (0 < card->getCardInfo()->ReqGreen - (m_CardStacksArray[2].m_Cards.Num() + m_TokenStacksArray[2].m_Tokens.Num()))
		goldTokenRequired += card->getCardInfo()->ReqGreen - (m_CardStacksArray[2].m_Cards.Num() + m_TokenStacksArray[2].m_Tokens.Num());
	if (0 < card->getCardInfo()->ReqRed - (m_CardStacksArray[3].m_Cards.Num() + m_TokenStacksArray[3].m_Tokens.Num()))
		goldTokenRequired += card->getCardInfo()->ReqRed - (m_CardStacksArray[3].m_Cards.Num() + m_TokenStacksArray[3].m_Tokens.Num());
	if (0 < card->getCardInfo()->ReqBlack - (m_CardStacksArray[4].m_Cards.Num() + m_TokenStacksArray[4].m_Tokens.Num()))
		goldTokenRequired += card->getCardInfo()->ReqBlack - (m_CardStacksArray[4].m_Cards.Num() + m_TokenStacksArray[4].m_Tokens.Num());
	
	return (m_TokenStacksArray[5].m_Tokens.Num() >= goldTokenRequired);
}

void ARodnelpsPlayerState::addCard_Implementation(ACard* card)
{
	if (card->isTaken() && !card->isReserved())
		return;
	card->setAsTaken();
	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();

	int32 colorIndex = int32(card->getCardInfo()->CardColor);
	int32 cardsInColor = m_CardStacksArray[colorIndex].m_Cards.Num();
	ATargetPoint* desiredPoint = getPlayerBoard()->getCardTargetPoints()[colorIndex];

	FRotator vectOrient = FRotator(0, -90.f * m_PlayerId, 0);
	moveActorOnBoard(card, desiredPoint->GetActorLocation() + vectOrient.RotateVector(FVector(-150.f * cardsInColor, 0.f, 8.f * cardsInColor)));

	payForCard(card);
	m_VictoryPoints += card->getCardInfo()->VictoryPoints;
	m_CardStacksArray[colorIndex].m_Cards.Push(card);
	if (card->isReserved())
	{
		card->setIsReservedStatus(false);
		m_ReservedCardArray[m_ReservedCardArray.Find(card)] = nullptr;
	}
	else
	{
		gamestate->getGameElementGenerator()->placeNewCard(card);
	}

	endTurn();
}

bool ARodnelpsPlayerState::addCard_Validate(ACard* card)
{
	return true;
}

bool ARodnelpsPlayerState::isTraderPosibbleToGet()
{
	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	for (const auto& trader : gamestate->getGameElementGenerator()->getTraderArray())
	{
		if (isMeetsTraderRequirements(trader))
		{
			return true;
		}
	}
	return false;
}

void ARodnelpsPlayerState::transferTrader_Implementation(ATraderCard* trader)
{
	if (trader->isTaken())
		return;
	if (m_TraderCardArray.Num() < 4)
	{
		ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
		moveActorOnBoard(trader, m_PlayerBoard->getTraderTargetPoints()[m_TraderCardArray.Num()]->GetActorLocation());
		trader->setIsTaken(true);
		m_VictoryPoints += 3;			//3 Victory points for a trader
		m_TraderCardArray.Push(trader);
		gamestate->getGameElementGenerator()->removeTrader(trader);
		m_isTakingTrader = true;
		endTurn();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Too many traders!!"));
	}
}

bool ARodnelpsPlayerState::transferTrader_Validate(ATraderCard* trader)
{
	return true;
}

bool ARodnelpsPlayerState::isMeetsTraderRequirements(ATraderCard* trader)
{
	if (trader->getTraderInfo()->ReqWhite <= m_CardStacksArray[0].m_Cards.Num() &&
		trader->getTraderInfo()->ReqBlue <= m_CardStacksArray[1].m_Cards.Num() &&
		trader->getTraderInfo()->ReqGreen <= m_CardStacksArray[2].m_Cards.Num() &&
		trader->getTraderInfo()->ReqRed <= m_CardStacksArray[3].m_Cards.Num() &&
		trader->getTraderInfo()->ReqBlack <= m_CardStacksArray[4].m_Cards.Num())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ARodnelpsPlayerState::payForCard(ACard* card)
{
	int32 numOfTokensToDiscard = card->getCardInfo()->ReqWhite - m_CardStacksArray[0].m_Cards.Num();
	payTokenStackForCard(numOfTokensToDiscard, 0);
	numOfTokensToDiscard = card->getCardInfo()->ReqBlue - m_CardStacksArray[1].m_Cards.Num();
	payTokenStackForCard(numOfTokensToDiscard, 1);
	numOfTokensToDiscard = card->getCardInfo()->ReqGreen - m_CardStacksArray[2].m_Cards.Num();
	payTokenStackForCard(numOfTokensToDiscard, 2);
	numOfTokensToDiscard = card->getCardInfo()->ReqRed - m_CardStacksArray[3].m_Cards.Num();
	payTokenStackForCard(numOfTokensToDiscard, 3);
	numOfTokensToDiscard = card->getCardInfo()->ReqBlack - m_CardStacksArray[4].m_Cards.Num();
	payTokenStackForCard(numOfTokensToDiscard, 4);
}

void ARodnelpsPlayerState::reserveCard_Implementation(ACard* card)
{
	int32 reservedCardIndex = 0;
	for (auto& cardField : m_ReservedCardArray)
	{
		if (cardField == nullptr)
		{
			cardField = card;
			break;
		}
		else
		{
			reservedCardIndex++;
		}
	}
	if (reservedCardIndex == 3)
	{
		broadcast_SendGuideMessage("You reserved max number of cards");
		return;
	}
		
	card->setIsReservedStatus(true);
	card->setAsTaken();

	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	ATargetPoint* desiredPoint = getPlayerBoard()->getReservedCardsTargetPoints()[reservedCardIndex];
	moveActorOnBoard(card, desiredPoint->GetActorLocation());

	if (gamestate->getGameElementGenerator()->getGoldTokenStack().Num() > 0)
	{
		AToken* goldToken = gamestate->getGameElementGenerator()->getGoldTokenStack().Last();
		addToken(goldToken);
		m_isTakingTokens = true;
	}

	gamestate->getGameElementGenerator()->placeNewCard(card);

	if (getTokenNum() > 10)
	{
		m_AreTokensDrawn = true;
		broadcast_SendGuideMessage("Discard excess tokens");
		broadcast_showMessageOnCenterOfScreen("Discard excess tokens", 0.5f);
	}
	else
	{
		resetTokenStatusAndEndTurn(gamestate);
	}
}

bool ARodnelpsPlayerState::reserveCard_Validate(ACard* card)
{
	return true;
}


void ARodnelpsPlayerState::moveActorOnBoard(AActor* actor, FVector desiredLocation)
{
	check(HasAuthority());

	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	gamestate->GetInterpolationManager()->setDesiredLocation(actor, actor->GetActorLocation() + FVector(0.f, 0.f, 500.f), 0.f);

	float roll = 0.f;
	float yaw = 0.f;
	if (Cast<ACard>(actor))
	{
		ACard* card = Cast<ACard>(actor);
		if (card->isOnTopOfDeck())
		{
			gamestate->GetInterpolationManager()->setDesiredRotation(actor, actor->GetActorRotation() + FRotator(0.f, 0.f, 180.f), 0.f);
			roll += 180.f;
		}
		if (m_ReservedCardArray.Contains(card))
		{
			yaw = card->GetActorRotation().Yaw;
		}
	}
	gamestate->GetInterpolationManager()->setDesiredRotation(actor, actor->GetActorRotation() + FRotator(0.f, -90.f*(m_PlayerId) - yaw, roll), 0.f);
	gamestate->GetInterpolationManager()->setDesiredLocation(actor, desiredLocation, 0.f);
}

bool ARodnelpsPlayerState::isTaken_Implementation()
{
	return true;
}

void ARodnelpsPlayerState::setTokenIndex_Implementation(AToken* token)
{
	token->setTokenIndex(m_TokenStacksArray[int32(token->getColor())].m_Tokens.Find(token) + 1);		//indexes from 1
}

void ARodnelpsPlayerState::setPlayerId(int32 id)
{
	check(HasAuthority());

	if (HasAuthority())
	{
		m_PlayerId = id;
	}
}

void ARodnelpsPlayerState::addVictoryPoints(int32 points)
{
	m_VictoryPoints += points;
}

void ARodnelpsPlayerState::broadcast_generateInfoOnchat_Implementation()
{
	if (ARodnelpsPlayerController* controller = Cast<ARodnelpsPlayerController>(GetOwner()))
	{
		if (controller->IsLocalController())
		{
			generateInfoOnchat();
		}
	}
}

void ARodnelpsPlayerState::generateInfoOnchat()
{
	sendGuideMessage("*******");
	sendGuideMessage("Id: " + UKismetStringLibrary::Conv_IntToString(m_PlayerId));
	sendGuideMessage("TokeNum: " + UKismetStringLibrary::Conv_IntToString(getTokenNum()));
	sendGuideMessage("IsTakingTokens: " + UKismetStringLibrary::Conv_BoolToString(m_isTakingTokens));
	sendGuideMessage("AreTokenDrown: " + UKismetStringLibrary::Conv_BoolToString(m_AreTokensDrawn));
	sendGuideMessage("*******");
}

void ARodnelpsPlayerState::broadcast_SendGuideMessage_Implementation(const FString &message)
{
	if (ARodnelpsPlayerController* controller = Cast<ARodnelpsPlayerController>(GetOwner()))
	{
		if (controller->IsLocalController())
		{
			sendGuideMessage(message);
		}
	}
}

void ARodnelpsPlayerState::broadcast_showMessageOnCenterOfScreen_Implementation(const FString& message, float messageTime)
{
	if (ARodnelpsPlayerController* controller = Cast<ARodnelpsPlayerController>(GetOwner()))
	{
		if (controller->IsLocalController())
		{
			showMessageOnCenterOfScreen(message, messageTime);
		}
	}
}

void ARodnelpsPlayerState::endTurn_Implementation()
{
	if (ARodnelpsGameMode* gameMode = GetWorld()->GetAuthGameMode<ARodnelpsGameMode>())
	{
		m_AreTokensDrawn = false;	//synch with server
		m_isTakingTokens = false;	//synch with server
		gameMode->endTurn();
	}
}

bool ARodnelpsPlayerState::endTurn_Validate()
{
	return true;
}

void ARodnelpsPlayerState::payTokenStackForCard(int32 tokensNum, int32 stackColorIndex)
{
	if (tokensNum <= 0 )
	{
		return;
	}
	while (tokensNum)
	{
		if (m_TokenStacksArray[stackColorIndex].m_Tokens.Num() == 0)
		{
			removeToken(m_TokenStacksArray[int32(ETokenColor::GOLD)].m_Tokens.Last());
		}
		else
		{
			removeToken(m_TokenStacksArray[stackColorIndex].m_Tokens.Last());
		}
		tokensNum--;
	}
}

bool ARodnelpsPlayerState::isPossibleToGetToken()
{
	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	AGameElementsGenerator* generator = gamestate->getGameElementGenerator();

	if (m_SecondTokenTakenColor == ETokenColor::MAX_COLOURS)
	{
		if (generator->getTokenStacksArray()[int32(m_FirstTokenTakenColor)].m_Tokens.Num() >= 3)
		{
			return true;
		}
		else
		{
			for (int32 i = 0; i < generator->getTokenStacksArray().Num(); i++)
			{
				if (generator->getTokenStacksArray()[i].m_Tokens.Num() >= 1)
				{
					if (i != int32(m_FirstTokenTakenColor) && i != int32(ETokenColor::GOLD))
					{
						return true;
					}
				}
			}
		}
	}
	else
	{
		for (int32 i = 0; i< generator->getTokenStacksArray().Num(); i++)
		{
			if (generator->getTokenStacksArray()[i].m_Tokens.Num() >= 1)
			{
				if (i != int32(m_FirstTokenTakenColor) && i != int32(m_SecondTokenTakenColor) && i != int32(ETokenColor::GOLD))
				{
					return true;
				}
			}
		}
	}
	return false;
}


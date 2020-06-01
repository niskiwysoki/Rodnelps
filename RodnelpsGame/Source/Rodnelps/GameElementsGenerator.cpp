// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElementsGenerator.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/DataTable.h"
#include "Card.h"
#include "TraderCard.h"
#include "set"
#include "Token.h"
#include "InterpolationManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "RodnelpsGameState.h"
#include "Net/UnrealNetwork.h"
#include "RodnelpsPlayerState.h"



// Sets default values
AGameElementsGenerator::AGameElementsGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

void AGameElementsGenerator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AGameElementsGenerator, m_DecksArray);
	DOREPLIFETIME(AGameElementsGenerator, m_TokenStacsArray);
}

// Called when the game starts or when spawned
void AGameElementsGenerator::BeginPlay()
{
	Super::BeginPlay();

	ARodnelpsGameState* gameState = GetWorld()->GetGameState<ARodnelpsGameState>();
	gameState->setGameElementGenerator(this);

	if(HasAuthority())
		generateGamePieces(gameState->getNumberOfPlayers());
}

void AGameElementsGenerator::LayOutTheCards()
{
	FVector newLocation = GetActorLocation() + FVector(0.f, -400.f, 0.f);
	AInterpolationManager* interpolMaganger = GetWorld()->SpawnActor<AInterpolationManager>(m_InterpolationManagerToSpawn, newLocation, FRotator::ZeroRotator);
	interpolMaganger->SetActorHiddenInGame(true);
	//starting time for players to connect
	interpolMaganger->setDesiredLocation(this, GetActorLocation(), 10.f);

	for (int32 deckIndex = 0; deckIndex < 3; ++deckIndex)
	{
		for (int32 cardIndex = 0; cardIndex < 4; ++cardIndex)	//cardIndex == 0 -> topCardOfDeck;
		{
			ACard* topDeckCard = m_DecksArray[deckIndex].m_Cards.Last();
			FVector targetLocation = GetActorLocation() + FVector(620.f + 620.f * deckIndex, 610.f + 500 * cardIndex, 300.f);
			FRotator targetRotation = topDeckCard->GetActorRotation() + FRotator(0.f, 0.f, 180.f);
			interpolMaganger->setDesiredLocation(topDeckCard, targetLocation, 0.f);
			interpolMaganger->setDesiredRotation(topDeckCard, targetRotation, 0.f);

			targetLocation = GetActorLocation() + FVector(620.f + 620.f * deckIndex, 610.f + 500 * cardIndex, GetActorLocation().Z - 100.f);
			interpolMaganger->setDesiredLocation(topDeckCard, targetLocation, 0.f);
			topDeckCard->setAsNotInDeck();

			m_DecksArray[deckIndex].m_Cards.Pop();
		}
		m_DecksArray[deckIndex].m_Cards.Last()->setIsOnTopOfDeck(true);
	}
}

void AGameElementsGenerator::generateTokens(int32 numberOfPlayers)
{

	int32 stacksNum = 5;		// gold stack is different
	int32 tokensInStack = 7;

	switch (numberOfPlayers)
	{
	case 1: tokensInStack = 4;  break;
	case 2: tokensInStack = 4;  break;
	case 3: tokensInStack = 5;  break;
	case 4: tokensInStack = 7;  break;
	default:  tokensInStack = 4; break;
	}

	float distanceBetweenStacks = 400.f;
	float heightBetweenTokens = 15.f;
	for (int32 i = 0; i < stacksNum; ++i)
	{
		FTokenArray tokenStack;
		for (int32 j = 0; j < tokensInStack; ++j)
		{
			FVector NewLocation = GetActorLocation() + FVector(0, 270.f + distanceBetweenStacks * i, heightBetweenTokens * j - 30.f);
			AToken* token = GetWorld()->SpawnActor<AToken>(m_TokenToSpawn, NewLocation, FRotator::ZeroRotator);
			token->setColor((ETokenColor)i);
			//token->setOwner(this);
			tokenStack.m_Tokens.Push(token);
		}
		m_TokenStacsArray.Push(tokenStack);
	}
	//Gold token stack
	FTokenArray tokenStack;
	for (int32 j = 0; j < 5; ++j)
	{
		FVector NewLocation = GetActorLocation() + FVector(0, 270.f + distanceBetweenStacks * stacksNum, heightBetweenTokens * j - 30.f);
		AToken* token = GetWorld()->SpawnActor<AToken>(m_TokenToSpawn, NewLocation, FRotator::ZeroRotator);
		token->setColor(ETokenColor::GOLD);
		//token->setOwner(this);
		tokenStack.m_Tokens.Push(token);
	}
	m_TokenStacsArray.Push(tokenStack);

	//setOwners+setIndexes
	for (auto& array : m_TokenStacsArray)
		for (auto& token : array.m_Tokens)
		{
			token->setOwner(this);
			token->SetTokenUI();
		}
}

void AGameElementsGenerator::addToken(AToken* token)
{
	ETokenColor color = token->getColor();

	if(HasAuthority())
	{ 
		ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
		gamestate->GetInterpolationManager()->setDesiredLocation(token, token->GetActorLocation() + FVector(0.f, 0.f, 500.f), 0.f);
		gamestate->GetInterpolationManager()->setDesiredRotation(token, token->GetActorRotation() + FRotator(0.f, 90.f*(gamestate->getActivePlayer()->getPlayerId()), 0.f), 0.f);    //
		gamestate->GetInterpolationManager()->setDesiredLocation(token, GetActorLocation() + FVector(0, 270.f + 400 * int32(color), 15 * m_TokenStacsArray[int32(color)].m_Tokens.Num() - 30.f), 0.f);
	}
	m_TokenStacsArray[int32(color)].m_Tokens.Push(token);
	token->setOwner(this);
}

void AGameElementsGenerator::removeToken(AToken* token)
{
	ETokenColor color = token->getColor();
	m_TokenStacsArray[int32(color)].m_Tokens.Pop();
}

int32 AGameElementsGenerator::getStackSize(AToken* token)
{
	return m_TokenStacsArray[int32(token->getColor())].m_Tokens.Num();
}

TArray<AToken*> AGameElementsGenerator::getGoldTokenStack()
{
	return m_TokenStacsArray[int32(ETokenColor::GOLD)].m_Tokens;
}

TArray<AToken*> AGameElementsGenerator::getTokenStack(AToken* token)
{
	return m_TokenStacsArray[int32(token->getColor())].m_Tokens;
}

const TArray<ATraderCard*>& AGameElementsGenerator::getTraderArray()
{
	return m_TradersArray;
}

void AGameElementsGenerator::generateGamePieces(int32 numberOfPlayers)
{
	check(HasAuthority());

	if (HasAuthority())
	{
		generateDecks(6.f, 620.f);
		generateTraders(numberOfPlayers, 500.f);
		generateTokens(numberOfPlayers);
		LayOutTheCards();
	}
}

void AGameElementsGenerator::removeTrader(ATraderCard* trader)
{
	m_TradersArray.Remove(trader);
}

void AGameElementsGenerator::placeNewCard(ACard* card)
{
	int32 deckIndex = card->getCardInfo()->CardTier - 1;
	if (m_DecksArray[deckIndex].m_Cards.Num() != 0)
	{
		ACard* topDeckCard = m_DecksArray[deckIndex].m_Cards.Last();
		if (!card->isOnTopOfDeck())
		{
			FVector targetLocation = card->GetActorLocation() + FVector(0.f, 0.f, 300.f);
			FRotator targetRotation = topDeckCard->GetActorRotation() + FRotator(0.f, 0.f, 180.f);
			ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
			gamestate->GetInterpolationManager()->setDesiredLocation(topDeckCard, targetLocation, 0.f);
			gamestate->GetInterpolationManager()->setDesiredRotation(topDeckCard, targetRotation, 0.f);
			targetLocation = card->GetActorLocation();
			gamestate->GetInterpolationManager()->setDesiredLocation(topDeckCard, targetLocation, 0.f);
		}
		topDeckCard->setAsNotInDeck();
		topDeckCard->setIsOnTopOfDeck(false);
		m_DecksArray[deckIndex].m_Cards.Pop();
		if (m_DecksArray[deckIndex].m_Cards.Num() != 0)
			m_DecksArray[deckIndex].m_Cards.Last()->setIsOnTopOfDeck(true);
	}
}

void AGameElementsGenerator::generateTraders(int32 numberOfPlayers, float distanceBetweenTraders)
{
	int32 TraderTableSize = m_TraderCards->GetRowMap().Num();
	std::set<int32> indexSet;
	for (int32 i = 0; i < TraderTableSize; ++i)
		indexSet.insert(i);
	int32 loopLimit = 0;
	while (indexSet.size() > numberOfPlayers + 1 && loopLimit < 10000) {
		indexSet.erase(FMath::RandRange(0, TraderTableSize - 1));
		loopLimit++;
	}

	int32 setValue = 0;
	int32 traderIndex = 0;
	for (const auto& traderStruct : m_TraderCards->GetRowMap())
	{
		if (indexSet.end() != indexSet.find(setValue))
		{
			FVector NewLocation = GetActorLocation() + FVector(2480.f, 100.f + distanceBetweenTraders * traderIndex, -10.f);
			ATraderCard* Trader = GetWorld()->SpawnActor<ATraderCard>(m_TraderToSpawn, NewLocation, FRotator::ZeroRotator);
			Trader->SetTraderInfo((FTraderSettings*)(traderStruct.Value));
			m_TradersArray.Push(Trader);
			traderIndex++;
		}
		setValue++;
	}
}

void AGameElementsGenerator::generateDecks(float cardHeightDiffrence, float distanceBetweenDecks)
{
	int32 deckIndex = 0;
	for (const auto& table : m_CardTablesArray)
	{
		FCardArray cardsArray;
		TArray<FCardSettings*> cardsStructsArray;
		
		for (const auto& cardStruct : table->GetRowMap())
		{
			cardsStructsArray.Push((FCardSettings*)(cardStruct.Value));
		}
		int32 cardIndex = 0;
		while (cardsStructsArray.Num() > 0)
		{
			FCardSettings* cardSettings = cardsStructsArray[FMath::RandRange(0, cardsStructsArray.Num() - 1)];
			FVector NewLocation = GetActorLocation() + FVector(620.f + distanceBetweenDecks * deckIndex, 100.f, cardHeightDiffrence * cardIndex - 30.f);
			ACard* card = GetWorld()->SpawnActor<ACard>(m_CardToSpawn, NewLocation, FRotator::ZeroRotator);
			card->setCardInfo(cardSettings);
			cardsArray.m_Cards.Push(card);
			cardsStructsArray.Remove(cardSettings);		//TODO Remove() removes all instances of item in array !!
			cardIndex++; 
		}
		m_DecksArray.Push(cardsArray);
		deckIndex++;
	}
}

bool AGameElementsGenerator::isTaken_Implementation()
{
	return false;
}

void AGameElementsGenerator::setTokenIndex_Implementation(AToken* token)
{
	token->setTokenIndex(m_TokenStacsArray[int32(token->getColor())].m_Tokens.Find(token) + 1);	//indexes from 1 
}

// Called every frame
void AGameElementsGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

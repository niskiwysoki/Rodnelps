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



// Sets default values
AGameElementsGenerator::AGameElementsGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AGameElementsGenerator::BeginPlay()
{
	Super::BeginPlay();

	ARodnelpsGameState* gameState = GetWorld()->GetGameState<ARodnelpsGameState>();
	gameState->setGameElementGenerator(this);


	/*int32 id = 0;
	for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter) {
		APlayerController* playerController = iter->Get();
		UGameplayStatics::SetPlayerControllerID(playerController, id);
		++id;
	}*/

	generateDecks(6.f, 620.f);
	generateTraders(500.f);
	generateTokens();
	LayOutTheCards();

}

void AGameElementsGenerator::LayOutTheCards()
{
	FVector newLocation = GetActorLocation() + FVector(0.f, -400.f, 0.f);
	AInterpolationManager* interpolMaganger = GetWorld()->SpawnActor<AInterpolationManager>(m_InterpolationManagerToSpawn, newLocation, FRotator::ZeroRotator);
	for (int32 deckIndex = 0; deckIndex < 3; ++deckIndex)
	{
		for (int32 cardIndex = 0; cardIndex < 4; ++cardIndex)	//cardIndex == 0 -> topCardOfDeck;
		{
			ACard* topDeckCard = m_DecksArray[deckIndex].Last();
			FVector targetLocation = GetActorLocation() + FVector(620.f + 620.f * deckIndex, 610.f + 500 * cardIndex, 300.f);
			FRotator targetRotation = topDeckCard->GetActorRotation() + FRotator(0.f, 0.f, 180.f);
			interpolMaganger->setDesiredLocation(topDeckCard, targetLocation, 0.f);
			interpolMaganger->setDesiredRotation(topDeckCard, targetRotation, 0.f);

			targetLocation = GetActorLocation() + FVector(620.f + 620.f * deckIndex, 610.f + 500 * cardIndex, GetActorLocation().Z - 100.f);
			interpolMaganger->setDesiredLocation(topDeckCard, targetLocation, 0.f);

			m_DecksArray[deckIndex].Pop();
		}
	}
}

void AGameElementsGenerator::generateTokens()
{
	int32 stacksNum = 5;		// gold stack is different
	int32 tokensInStack = 7;
	float distanceBetweenStacks = 400.f;
	float heightBetweenTokens = 15.f;
	for (int32 i = 0; i < stacksNum; ++i)
	{
		TArray<AToken*> tokenStack;
		for (int32 j = 0; j < tokensInStack; ++j)
		{
			FVector NewLocation = GetActorLocation() + FVector(0, 270.f + distanceBetweenStacks * i, heightBetweenTokens * j - 30.f);
			AToken* token = GetWorld()->SpawnActor<AToken>(m_TokenToSpawn, NewLocation, FRotator::ZeroRotator);
			token->setColor((ETokenColor)i);
			token->setOwner(this);
			tokenStack.Push(token);
		}
		m_TokenStacsArray.Push(tokenStack);
	}
	//Gold token stack
	TArray<AToken*> tokenStack;
	for (int32 j = 0; j < 5; ++j)
	{
		FVector NewLocation = GetActorLocation() + FVector(0, 270.f + distanceBetweenStacks * stacksNum, heightBetweenTokens * j - 30.f);
		AToken* token = GetWorld()->SpawnActor<AToken>(m_TokenToSpawn, NewLocation, FRotator::ZeroRotator);
		token->setColor((ETokenColor)stacksNum);  // StacksNum = i = 5
		token->setOwner(this);
		tokenStack.Push(token);
	}
	m_TokenStacsArray.Push(tokenStack);
}

void AGameElementsGenerator::addToken(AToken* token)
{
	ETokenColor color = token->getColor();

	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	gamestate->GetInterpolationManager()->setDesiredLocation(token, token->GetActorLocation() + FVector(0.f, 0.f, 500.f), 0.f);
	gamestate->GetInterpolationManager()->setDesiredLocation(token, GetActorLocation() + FVector(0, 270.f + 400 * int32(color), 15 * m_TokenStacsArray[int32(color)].Num() - 30.f), 0.f);

	m_TokenStacsArray[int32(color)].Push(token);
	token->setOwner(this);
}

void AGameElementsGenerator::removeToken(AToken* token)
{
	ETokenColor color = token->getColor();
	m_TokenStacsArray[int32(color)].Pop();
}

void AGameElementsGenerator::generateTraders(float distanceBetweenTraders)
{
	int32 TraderTableSize = m_TraderCards->GetRowMap().Num();
	std::set<int32> indexSet;
	for (int32 i = 0; i < TraderTableSize; ++i)
		indexSet.insert(i);
	int32 loopLimit = 0;
	while (indexSet.size() > 5 && loopLimit < 10000) {
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
		TArray<ACard*> cardsArray;
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
			ACard* Card = GetWorld()->SpawnActor<ACard>(m_CardToSpawn, NewLocation, FRotator::ZeroRotator);
			Card->setCardInfo(cardSettings);
			cardsArray.Push(Card);
			cardsStructsArray.Remove(cardSettings);
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

// Called every frame
void AGameElementsGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

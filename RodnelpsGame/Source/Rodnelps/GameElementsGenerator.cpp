// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElementsGenerator.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/DataTable.h"
#include "Card.h"
#include "TraderCard.h"
#include "set"
#include "Token.h"
#include "InterpolationManager.h"


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

	generateDecks(6.f, 620.f);
	generateTraders(470.f);
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
	int32 StacksNum = 5;		// gold stack is diffrent
	int32 tokensInStack = 7;
	float distanceBetweenStacks = 400.f;
	float heightBetweenTokens = 15.f;
	for (int32 i = 0; i < StacksNum; ++i)
	{
		for (int32 j = 0; j < tokensInStack; ++j)
		{
			FVector NewLocation = GetActorLocation() + FVector(0, 270.f + distanceBetweenStacks * i, heightBetweenTokens * j - 30.f);
			AToken* token = GetWorld()->SpawnActor<AToken>(m_TokenToSpawn, NewLocation, FRotator::ZeroRotator);
			token->setColor((ETokenColor)i);
		}
	}

	for (int32 j = 0; j < 5; ++j)
	{
		FVector NewLocation = GetActorLocation() + FVector(0, 270.f + distanceBetweenStacks * StacksNum, heightBetweenTokens * j - 30.f);
		AToken* token = GetWorld()->SpawnActor<AToken>(m_TokenToSpawn, NewLocation, FRotator::ZeroRotator);
		token->setColor((ETokenColor)StacksNum);  // StacksNum = i = 5
	}
}

void AGameElementsGenerator::generateTraders(float distanceBetweenTraders)
{

	int32 TraderTableSize = m_TraderCards->GetRowMap().Num();
	std::set<int32> indexSet;
	for (int32 i = 0; i < TraderTableSize; ++i)
		indexSet.insert(i);
	int32 loopLimit = 0;
	while (indexSet.size() > 4 && loopLimit < 10000) {
		indexSet.erase(FMath::RandRange(0, TraderTableSize - 1));
		loopLimit++;
	}

	int32 setValue = 0;
	int32 traderIndex = 0;
	for (const auto& traderStruct : m_TraderCards->GetRowMap())
	{
		
		if (indexSet.end() != indexSet.find(setValue))
		{
			FVector NewLocation = GetActorLocation() + FVector(500.f + distanceBetweenTraders * traderIndex, 2600.f, -10.f);
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
		int32 cardIndex = 0;
		for (const auto& cardStruct : table->GetRowMap())
		{
			FVector NewLocation = GetActorLocation() + FVector(620.f + distanceBetweenDecks * deckIndex, 100.f, cardHeightDiffrence * cardIndex - 30.f);
			ACard* Card = GetWorld()->SpawnActor<ACard>(m_CardToSpawn, NewLocation, FRotator::ZeroRotator);
			Card->setCardInfo((FCardSettings*)(cardStruct.Value));
			cardsArray.Push(Card);
			cardIndex++;
		}
		m_DecksArray.Push(cardsArray);
		deckIndex++;
	}
}

// Called every frame
void AGameElementsGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElementsGenerator.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/DataTable.h"
#include "Card.h"
#include "TraderCard.h"
#include "set"


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
			FVector NewLocation = GetActorLocation() + FVector(500.f + distanceBetweenTraders * traderIndex, 2200.f, 0.f);
			ATraderCard* Trader = GetWorld()->SpawnActor<ATraderCard>(m_TraderToSpawn, NewLocation, FRotator::ZeroRotator);
			Trader->SetTraderInfo((FTraderSettings*)(traderStruct.Value));
			traderIndex++;
		}
		setValue++;
	}
}

void AGameElementsGenerator::generateDecks(float cardHeightDiffrence, float distanceBetweenDecks)
{
	int32 j = 1;
	for (const auto& table : m_CardTablesArray)
	{
		int32 i = 0;
		for (const auto& cardStruct : table->GetRowMap())
		{
			FVector NewLocation = GetActorLocation() + FVector(distanceBetweenDecks * j, 100.f, cardHeightDiffrence * i);
			ACard* Card = GetWorld()->SpawnActor<ACard>(m_CardToSpawn, NewLocation, FRotator::ZeroRotator);
			Card->SetCardInfo((FCardSettings*)(cardStruct.Value));
			i++;
		}
		j++;
	}
}

// Called every frame
void AGameElementsGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

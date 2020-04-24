// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBoardSpace.h"
#include "Kismet/GameplayStatics.h"
#include "RodnelpsGameState.h"

// Sets default values
APlayerBoardSpace::APlayerBoardSpace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerBoardSpace::BeginPlay()
{
	Super::BeginPlay();
	if (m_PlayerId == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Set id to instance of PlayerBoardSpace"))
	}
	else
	{
		ARodnelpsGameState* gameState = GetWorld()->GetGameState<ARodnelpsGameState>();
		gameState->addPlayer(this);
	}
	
}

// Called every frame
void APlayerBoardSpace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 APlayerBoardSpace::getPlayerId()
{
	return m_PlayerId;
}

TArray<ATargetPoint*> APlayerBoardSpace::getCardTargetPoints()
{
	return m_CardsTargetPoints;
}

TArray<ATargetPoint*> APlayerBoardSpace::getReservedCardsTargetPoints()
{
	return m_ReservedCardsTargetPoints;
}

TArray<ATargetPoint*> APlayerBoardSpace::getTokenTargetPoints()
{
	return m_TokensTargetPoints;
}

TArray<ATargetPoint*> APlayerBoardSpace::getTraderTargetPoints()
{
	return m_TradersTargetPoints;
}


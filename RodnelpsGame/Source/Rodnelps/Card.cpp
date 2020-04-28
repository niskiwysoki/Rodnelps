// Fill out your copyright notice in the Description page of Project Settings.

#include "Card.h"
#include "RodnelpsGameState.h"
#include "RodnelpsPlayerState.h"
#include "InterpolationManager.h"
#include "Engine/TargetPoint.h"
#include "InputCoreTypes.h"

// Sets default values
ACard::ACard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_CardSettings = nullptr;

}

// Called when the game starts or when spawned
void ACard::BeginPlay()
{
	Super::BeginPlay();
	m_CardSettings = new FCardSettings();

	OnClicked.AddDynamic(this, &ACard::onSelected);
}

void ACard::onSelected(AActor* Target, FKey ButtonPressed)
{
	if (EKeys::LeftMouseButton == ButtonPressed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Card location: %s"), *this->GetActorLocation().ToString())
		UE_LOG(LogTemp, Warning, TEXT("Info: VP %d; CC %d; RW %d; RB %d; RG %d; RR %d; RB %d; CT %d"),
			m_CardSettings->VictoryPoints, m_CardSettings->CardColor, m_CardSettings->ReqWhite, m_CardSettings->ReqBlue,
			m_CardSettings->ReqGreen, m_CardSettings->ReqRed, m_CardSettings->ReqBlack, m_CardSettings->CardTier)

		ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
		ARodnelpsPlayerState* activePlayer = gamestate->getActivePlayer();
		if (activePlayer->GetPawn()->IsLocallyControlled())
		{
			gamestate->GetInterpolationManager()->setDesiredLocation(this, GetActorLocation() + FVector(0.f, 0.f, 500.f), 0.f);
			this->m_CardSettings->CardColor;
			TArray<ATargetPoint*> cardsTargetPoints = activePlayer->getPlayerBoard()->getCardTargetPoints();
			ATargetPoint* desiredPoint = cardsTargetPoints[(int32)this->m_CardSettings->CardColor];
			gamestate->GetInterpolationManager()->setDesiredLocation(this, desiredPoint->GetActorLocation(), 0.f);
		}
	}
	
	if (EKeys::RightMouseButton == ButtonPressed)
	{

	}
}

void ACard::setCardInfo(FCardSettings* CardInfo)
{
	m_CardSettings = CardInfo;
}

// Called every frame
void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


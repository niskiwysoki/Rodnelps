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
	m_isTaken = 0;
	m_isReserved = 0;

}

// Called when the game starts or when spawned
void ACard::BeginPlay()
{
	Super::BeginPlay();
	m_CardSettings = new FCardSettings();

	OnClicked.AddDynamic(this, &ACard::onSelected);
}

void ACard::logOutCardInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("Card location: %s"), *this->GetActorLocation().ToString())
	UE_LOG(LogTemp, Warning, TEXT("Info: VP %d; CC %d; RW %d; RB %d; RG %d; RR %d; RB %d; CT %d"),
		m_CardSettings->VictoryPoints, m_CardSettings->CardColor, m_CardSettings->ReqWhite, m_CardSettings->ReqBlue,
		m_CardSettings->ReqGreen, m_CardSettings->ReqRed, m_CardSettings->ReqBlack, m_CardSettings->CardTier)
}

void ACard::onSelected(AActor* Target, FKey ButtonPressed)
{
	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	ARodnelpsPlayerState* activePlayer = gamestate->getActivePlayer();
	logOutCardInfo();

	if (EKeys::LeftMouseButton == ButtonPressed)
	{
		if (activePlayer->GetPawn()->IsLocallyControlled())
		{
			if (!isTaken() || isReserved())			//every card reserved is taken
			{
				if (activePlayer->areCardRequirementsFulfilled(this))
				{
					activePlayer->addCard(this);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("You don't meet card requirements"))
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("This card is taken"))
			}
		}
	}
	
	if (EKeys::RightMouseButton == ButtonPressed)
	{
		if (!isTaken())
		{
			activePlayer->reserveCard(this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("You cannot reserved taken card from board"))
		}
	}
}

void ACard::setCardInfo(FCardSettings* CardInfo)
{
	m_CardSettings = CardInfo;
}

FCardSettings* ACard::getCardInfo()
{
	return m_CardSettings;
}

void ACard::setAsTaken()
{
	m_isTaken = true;
}

bool ACard::isTaken()
{
	return m_isTaken;
}

bool ACard::isReserved()
{
	return m_isReserved;
}

void ACard::setAsReserved()
{
	m_isReserved = true;
}

void ACard::setAsNotReserved()
{
	m_isReserved = false;
}

// Called every frame
void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


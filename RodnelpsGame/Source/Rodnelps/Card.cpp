// Fill out your copyright notice in the Description page of Project Settings.

#include "Card.h"
#include "RodnelpsGameState.h"
#include "RodnelpsPlayerState.h"
#include "InterpolationManager.h"
#include "Engine/TargetPoint.h"
#include "InputCoreTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/CardUserWidget.h"

// Sets default values
ACard::ACard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_CardSettings = nullptr;
	m_isTaken = 0;
	m_isReserved = 0;
	m_IsInDeck = 1;
	m_IsOnTopOfDeck = 0;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = m_Mesh;

	m_WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	m_WidgetComp->SetupAttachment(m_Mesh);

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

	if (activePlayer->GetPawn()->IsLocallyControlled())
	{
		if (!activePlayer->isTakingTokens())
		{
			if (!activePlayer->isTakingTraders())
			{
				if (EKeys::LeftMouseButton == ButtonPressed)
				{
					if (!this->isInDeck())
					{
						if (!isTaken() || isReserved())			// if every card reserved is taken
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
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("You can't buy card from deck"))
					}
				}

				if (EKeys::RightMouseButton == ButtonPressed)
				{
					if (!isTaken())
					{
						if (!isInDeck() || isOnTopOfDeck())
						{
							activePlayer->reserveCard(this);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("You can reserve card only from top of deck"))
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("You cannot reserve taken card from player's board"))
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Before end of turn take one of avaliable trader"))
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("You are drawing or discarding tokens now"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn is not locally controlled"));
	}
}

void ACard::setCardInfo(FCardSettings* CardInfo)
{
	m_CardSettings = CardInfo;

	if (m_WidgetComp)
	{
		UCardUserWidget* Widget = Cast<UCardUserWidget>(m_WidgetComp->GetUserWidgetObject());

		if (Widget)
			Widget->SetCard(this);
		else
			UE_LOG(LogTemp, Warning, TEXT("Widget class does not inherit from UCardUserWidget!"));
	}
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

void ACard::setIsReservedStatus(bool status)
{
	m_isReserved = status;
}

bool ACard::isInDeck()
{
	return m_IsInDeck;
}

bool ACard::isOnTopOfDeck()
{
	return m_IsOnTopOfDeck;
}

void ACard::setIsOnTopOfDeck(bool status)
{
	m_IsOnTopOfDeck = status;
}

void ACard::setAsNotInDeck()
{
	m_IsInDeck = false;
}

// Called every frame
void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


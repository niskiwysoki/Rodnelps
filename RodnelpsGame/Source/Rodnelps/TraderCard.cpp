// Fill out your copyright notice in the Description page of Project Settings.


#include "TraderCard.h"
#include "RodnelpsGameState.h"
#include "RodnelpsPlayerState.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/TraderUserWidget.h"

// Sets default values
ATraderCard::ATraderCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_TraderSettings = nullptr;
	m_isTaken = 0;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = m_Mesh;

	m_WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	m_WidgetComp->SetupAttachment(m_Mesh);
}

// Called when the game starts or when spawned
void ATraderCard::BeginPlay()
{
	Super::BeginPlay();
	m_TraderSettings = new FTraderSettings();

	OnClicked.AddDynamic(this, &ATraderCard::OnSelected);
}

void ATraderCard::OnSelected(AActor* Target, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("TraderCard location: %s"), *this->GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Info: RW %d; RB %d; RG %d; RR %d; RB %d"),
		m_TraderSettings->ReqWhite, m_TraderSettings->ReqBlue, m_TraderSettings->ReqGreen, m_TraderSettings->ReqRed, m_TraderSettings->ReqBlack);

	ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
	ARodnelpsPlayerState* activePlayer = gamestate->getActivePlayer();

	if (activePlayer->GetPawn()->IsLocallyControlled())
	{
		if (activePlayer->isMeetsTraderRequirements(this))
		{
			if (!isTaken())
			{
				if (activePlayer->isTakingTraders())
				{
					activePlayer->transferTrader(this);
					setIsTaken(true);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("You can only choose trader if is multiple choice"))
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Trader is already taken"))
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("You don't meet requirements"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn is not locally controlled"))
	}
}

void ATraderCard::SetTraderInfo(FTraderSettings* TraderInfo)
{
	m_TraderSettings = TraderInfo;
	if (m_WidgetComp)
	{
		UTraderUserWidget* Widget = Cast<UTraderUserWidget>(m_WidgetComp->GetUserWidgetObject());

		if (Widget)
			Widget->setTrader(this);
		else
			UE_LOG(LogTemp, Warning, TEXT("Widget class does not inherit from UTokenUserWidget!"));
	}
}

FTraderSettings* ATraderCard::getTraderInfo()
{
	return m_TraderSettings;
}

bool ATraderCard::isTaken()
{
	return m_isTaken;
}

void ATraderCard::setIsTaken(bool status)
{
	m_isTaken = status;
}

// Called every frame
void ATraderCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


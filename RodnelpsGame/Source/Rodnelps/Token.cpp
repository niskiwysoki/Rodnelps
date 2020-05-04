// Fill out your copyright notice in the Description page of Project Settings.


#include "Token.h"
#include "Engine/Engine.h"
#include "RodnelpsGameState.h"
#include "RodnelpsPlayerState.h"
#include "InterpolationManager.h"

// Sets default values
AToken::AToken() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Color = ETokenColor::GOLD;
	
}

// Called when the game starts or when spawned
void AToken::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &AToken::OnSelected);
}

void AToken::OnSelected(AActor* Target, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("Token location: %s"), *this->GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Token color: %d"), m_Color);

	if (m_Owner->Implements<UOwnershipInterface>())
	{
		ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
		ARodnelpsPlayerState* activePlayer = gamestate->getActivePlayer();
		if (activePlayer->GetPawn()->IsLocallyControlled())
		{
			if (!activePlayer->isTakingTraders())
			{
				if (!IOwnershipInterface::Execute_isTaken(m_Owner))
				{
					if (!activePlayer->areTokensDrawn())
					{
						if (getColor() != ETokenColor::GOLD)
						{
							activePlayer->addStandardToken(this);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Reserve card with RMB to get gold token"));
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("You have too many tokens. Discard excess tokens"));
					}
				}
				else
				{
					if (activePlayer->getTokenNum() > 10 && activePlayer->areTokensDrawn())
					{
						activePlayer->removeToken(this);
						if (activePlayer->getTokenNum() <= 10)
						{
							activePlayer->resetTokenStatusAndEndTurn(gamestate);
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Token is already taken"));
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
			UE_LOG(LogTemp, Warning, TEXT("Pawn is not locally controlled"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner does not implement IOwnershipInterface"))
	}
}

void AToken::setColor(ETokenColor tokenColor)
{
	m_Color = tokenColor;
}

ETokenColor AToken::getColor()
{
	return m_Color;
}

void AToken::setOwner(UObject* newOwner)
{
	m_Owner = newOwner;
}

// Called every frame
void AToken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




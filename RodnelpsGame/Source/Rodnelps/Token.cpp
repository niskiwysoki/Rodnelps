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
	m_IsTaken = 0;
	
}


// Called when the game starts or when spawned
void AToken::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &AToken::OnSelected);
}

void AToken::OnSelected(AActor* Target, FKey ButtonPressed)
{
	if (!isTaken())
	{
		ARodnelpsGameState* gamestate = GetWorld()->GetGameState<ARodnelpsGameState>();
		ARodnelpsPlayerState* activePlayer = gamestate->getActivePlayer();
		if (activePlayer->GetPawn()->IsLocallyControlled())
		{
			if (activePlayer->getTokenNum() < 10)
			{
				activePlayer->addToken(this);

				/*gamestate->GetInterpolationManager()->setDesiredLocation(this, GetActorLocation() + FVector(0.f, 0.f, 500.f), 0.f);
				this->m_CardSettings->CardColor;
				TArray<ATargetPoint*> cardsTargetPoints = activePlayer->getPlayerBoard()->getCardTargetPoints();
				ATargetPoint* desiredPoint = cardsTargetPoints[(int32)this->m_CardSettings->CardColor];
				gamestate->GetInterpolationManager()->setDesiredLocation(this, desiredPoint->GetActorLocation(), 0.f);*/
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("You have too many tokens"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Token is already taken"));
	}
	

	UE_LOG(LogTemp, Warning, TEXT("Token location: %s"), *this->GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Token color: %d"), m_Color);
}

void AToken::setColor(ETokenColor tokenColor)
{
	m_Color = tokenColor;
}

ETokenColor AToken::getColor()
{
	return m_Color;
}

void AToken::setOwner(IOwnershipInterface* newOwner)
{
	m_Owner = newOwner;
}

// Called every frame
void AToken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AToken::isTaken()
{
	return m_IsTaken;
}


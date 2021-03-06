// Fill out your copyright notice in the Description page of Project Settings.


#include "Token.h"
#include "Engine/Engine.h"
#include "RodnelpsGameState.h"
#include "RodnelpsPlayerState.h"
#include "InterpolationManager.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Components/WidgetComponent.h"
#include "UI/TokenUserWidget.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AToken::AToken() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Color = ETokenColor::MAX_COLOURS;
	m_TokenIndex = 0;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = m_Mesh;
	m_WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	m_WidgetComp->SetupAttachment(m_Mesh);

	m_Owner = nullptr;

	bReplicates = true;
	SetReplicatingMovement(true);
}

void AToken::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AToken, m_Color);
	DOREPLIFETIME(AToken, m_Owner);
	DOREPLIFETIME(AToken, m_TokenIndex);
}

// Called when the game starts or when spawned
void AToken::BeginPlay()
{
	Super::BeginPlay();

	if (m_WidgetComp)
	{
		UTokenUserWidget* Widget = Cast<UTokenUserWidget>(m_WidgetComp->GetUserWidgetObject());
		if (Widget)
			Widget->setToken(this);
		else
			UE_LOG(LogTemp, Warning, TEXT("Widget class does not inherit from UTokenUserWidget!"));
	}

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
		if (activePlayer->GetPawn() && activePlayer->GetPawn()->IsLocallyControlled())
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
							activePlayer->sendGuideMessage("Reserve card with RMB to get gold token");
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("You have too many tokens. Discard excess tokens"));
						activePlayer->sendGuideMessage("You have too many tokens. Discard excess tokens");
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
						activePlayer->sendGuideMessage("Token is already taken");
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Before end of turn take one of avaliable trader"))
				activePlayer->sendGuideMessage("Before end of turn take one of available trader");
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pawn is not locally controlled"));
			activePlayer->sendGuideMessage("Wait for your turn");
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
	onColorRep();
}

void AToken::onColorRep()
{
	setMaterial(m_TokenMaterialsArray[(int)m_Color]);
}

void AToken::setMaterial(UMaterial* material)
{
	m_Mesh->SetMaterial(0, material);
}

ETokenColor AToken::getColor()
{
	return m_Color;
}

void AToken::setOwner(UObject* newOwner)
{
	m_Owner = newOwner;
}

void AToken::SetTokenUI()
{
	check(HasAuthority());

	if (m_Owner->Implements<UOwnershipInterface>())
	{
		IOwnershipInterface::Execute_setTokenIndex(m_Owner, this);
	}

	Broadcast_SetTokenUI();
}

void AToken::Broadcast_SetTokenUI_Implementation()
{
	UTokenUserWidget* Widget = Cast<UTokenUserWidget>(m_WidgetComp->GetUserWidgetObject());
	if (Widget)
		Widget->OnTokenSet(this);
	else
		UE_LOG(LogTemp, Warning, TEXT("Widget class does not inherit from UTokenUserWidget!"));
}

void AToken::setTokenIndex(int32 index)
{
	m_TokenIndex = index;
}

// Called every frame
void AToken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




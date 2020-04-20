// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"

// Sets default values
ACard::ACard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_CardSettings = nullptr;

	//m_InterpolationSpeed = 5.f;
	//m_DesiredLocRotDelay = 10.f;
}

// Called when the game starts or when spawned
void ACard::BeginPlay()
{
	Super::BeginPlay();
	m_CardSettings = new FCardSettings();
	//m_DesiredLocation = GetActorLocation();		// in begin it must be
	//m_DesiredRotation = GetActorRotation();

	OnClicked.AddDynamic(this, &ACard::onSelected);
}

void ACard::onSelected(AActor* Target, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("Card location: %s"), *this->GetActorLocation().ToString())
	UE_LOG(LogTemp, Warning, TEXT("Info: VP %d; CC %d; RW %d; RB %d; RG %d; RR %d; RB %d; CT %d"), m_CardSettings->VictoryPoints, m_CardSettings->CardColor, m_CardSettings->ReqWhite, m_CardSettings->ReqBlue, m_CardSettings->ReqGreen, m_CardSettings->ReqRed, m_CardSettings->ReqBlack, m_CardSettings->CardTier)

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


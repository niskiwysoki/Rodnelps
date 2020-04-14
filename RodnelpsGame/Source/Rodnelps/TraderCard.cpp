// Fill out your copyright notice in the Description page of Project Settings.


#include "TraderCard.h"

// Sets default values
ATraderCard::ATraderCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_TraderSettings = nullptr;
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

}

void ATraderCard::SetTraderInfo(FTraderSettings* TraderInfo)
{
	m_TraderSettings = TraderInfo;
}

// Called every frame
void ATraderCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


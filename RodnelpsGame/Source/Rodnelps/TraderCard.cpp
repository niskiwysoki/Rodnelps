// Fill out your copyright notice in the Description page of Project Settings.


#include "TraderCard.h"

// Sets default values
ATraderCard::ATraderCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATraderCard::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &ATraderCard::OnSelected);
}

void ATraderCard::OnSelected(AActor* Target, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("TraderCard location: %s"), *this->GetActorLocation().ToString());
}

// Called every frame
void ATraderCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


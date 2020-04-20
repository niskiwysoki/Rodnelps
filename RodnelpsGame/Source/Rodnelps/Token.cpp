// Fill out your copyright notice in the Description page of Project Settings.


#include "Token.h"
#include "Engine/Engine.h"

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
}

void AToken::setColor(ETokenColor tokenColor)
{
	m_Color = tokenColor;
}

// Called every frame
void AToken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


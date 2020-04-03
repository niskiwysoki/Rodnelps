// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TraderCard.generated.h"



UCLASS()
class RODNELPS_API ATraderCard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATraderCard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSelected(AActor* Target, FKey ButtonPressed);

private:
	struct TraderRequirements
	{
		int WhiteCards;
		int BlueCards;
		int GreenCards;
		int RedCards;
		int BlackCards;
	};

	TraderRequirements m_TraderRequirements;
};

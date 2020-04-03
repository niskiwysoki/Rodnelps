// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Card.generated.h"

enum class ECardColor
{
	WHITE,
	BLUE,
	GREEN,
	RED,
	BLACK,
	MAX_COLORS
};

struct Requirments
{
	int WhiteColors;
	int BlueColors;
	int GreenColors;
	int RedColors;
	int BlackColors;
};



UCLASS()
class RODNELPS_API ACard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSelected(AActor* Target, FKey ButtonPressed);

private:

	struct CardInfo
	{
		int m_VictoryPoints;
		ECardColor m_CardColor;
		Requirments m_Requirements;
		int m_CardTier;			// from 1 do 3;
	};
	CardInfo m_CardInfo;
};

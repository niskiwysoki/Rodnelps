// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Token.generated.h"

class IOwnershipInterface;

enum class ETokenColor : int32
{
	WHITE,
	BLUE,
	GREEN,
	RED,
	BLACK,
	GOLD,
	MAX_COLOURS
};


UCLASS()
class RODNELPS_API AToken : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToken();

	bool isTaken();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSelected(AActor* Target, FKey ButtonPressed);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void setColor(ETokenColor tokenColor);

	ETokenColor getColor();

	void setOwner(IOwnershipInterface* newOwner);

private:
	ETokenColor m_Color;
	bool m_IsTaken;
	IOwnershipInterface* m_Owner;
};

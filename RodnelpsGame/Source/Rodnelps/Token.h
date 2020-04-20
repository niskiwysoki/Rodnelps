// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Token.generated.h"

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSelected(AActor* Target, FKey ButtonPressed);

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void setColor(ETokenColor tokenColor);
private:
	ETokenColor m_Color;
};

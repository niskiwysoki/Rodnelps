// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBoardSpace.generated.h"

class ATargetPoint;

UCLASS()
class RODNELPS_API APlayerBoardSpace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerBoardSpace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 getPlayerId();

	TArray<ATargetPoint*> getCardTargetPoints();
	TArray<ATargetPoint*> getReservedCardsTargetPoints();
	TArray<ATargetPoint*> getTokenTargetPoints();
	TArray<ATargetPoint*> getTraderTargetPoints();

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "BoardLayout", meta = (AllowPrivateAccess = "true"))
		TArray<ATargetPoint*> m_CardsTargetPoints;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "BoardLayout", meta = (AllowPrivateAccess = "true"))
		TArray<ATargetPoint*> m_ReservedCardsTargetPoints;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "BoardLayout", meta = (AllowPrivateAccess = "true"))
		TArray<ATargetPoint*> m_TokensTargetPoints;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "BoardLayout", meta = (AllowPrivateAccess = "true"))
		TArray<ATargetPoint*> m_TradersTargetPoints;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "BoardLayout", meta = (AllowPrivateAccess = "true"))
		int32 m_PlayerId = -1;

};

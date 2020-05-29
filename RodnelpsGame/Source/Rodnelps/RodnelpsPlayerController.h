// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RodnelpsPlayerController.generated.h"

class ARodnelpsPlayerState;

USTRUCT(BlueprintType)
struct FPlayerScoreInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "PlayerInfo")
	int32 VictoryPoints = 0;

	UPROPERTY(BlueprintReadOnly, Category = "PlayerInfo")
	TArray<int32> NumbersOfTokensArray;

	UPROPERTY(BlueprintReadOnly, Category = "PlayerInfo")
	TArray<int32> NumbersOfCardsArray;

};


UCLASS()
class RODNELPS_API ARodnelpsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARodnelpsPlayerController();


	void setPlayerScore(ARodnelpsPlayerState* playerState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FPlayerScoreInfo& getPlayerScore() const { return m_PlayerScore; }

protected:
	

private:
	FPlayerScoreInfo m_PlayerScore;
};

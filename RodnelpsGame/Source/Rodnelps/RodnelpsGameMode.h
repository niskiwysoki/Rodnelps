// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RodnelpsGameMode.generated.h"

class APlayerBoardSpace;
class ARodnelpsPlayerState;

UCLASS()
class RODNELPS_API ARodnelpsGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	void RegisterPlayerState(ARodnelpsPlayerState* playerState);

	void registerPlayerBoard(APlayerBoardSpace* board);
	APlayerBoardSpace* GetPlayerBoard(int32 id);

	virtual void HandleMatchHasStarted() override;
	
	void setActivePlayer(ARodnelpsPlayerState* player);

	void setNextActivePlayer();
	void endTurn();
	void gameSummary();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GlobalEvents")
	void UpdateScoreTableBP();

protected:

	TArray<APlayerBoardSpace*> m_BoardArray;
	TArray<ARodnelpsPlayerState*> m_PlayersArray;
	
	ARodnelpsPlayerState* m_ActivePlayer;

private:
	int32 getPlayerCardSum(ARodnelpsPlayerState* player);
	void UpdateScoreTable();

	bool m_IsLastRound;
	static int32 s_IdCounter;
	
};

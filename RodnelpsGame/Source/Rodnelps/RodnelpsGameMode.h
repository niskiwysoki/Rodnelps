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

	//ARodnelpsGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	void RegisterPlayerState(ARodnelpsPlayerState* playerState);

	void registerPlayerBoard(APlayerBoardSpace* board);
	APlayerBoardSpace* GetPlayerBoard(int32 id);

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	//virtual void HandleMatchIsWaitingToStart() override;

	virtual void Tick(float DeltaTime) override;

	void setActivePlayer(ARodnelpsPlayerState* player);

	void setNextActivePlayer();
	void endTurn();
	void gameSummary();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GlobalEvents")
	void UpdateScoreTableBP();

	int32 getPlayersNumber() const { return m_PlayerNumber; }

protected:

	TArray<APlayerBoardSpace*> m_BoardArray;
	TArray<ARodnelpsPlayerState*> m_PlayersArray;
	
	ARodnelpsPlayerState* m_ActivePlayer;

private:
	int32 getPlayerCardSum(ARodnelpsPlayerState* player);
	void UpdateScoreTable();
	void wait(float time);

	bool m_IsLastRound;
	static int32 s_IdCounter;
	float m_Time;
	
	int32 m_PlayerNumber;
	
};

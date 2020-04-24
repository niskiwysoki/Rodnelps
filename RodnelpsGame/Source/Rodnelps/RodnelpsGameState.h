// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "OwnershipInterface.h"
#include "RodnelpsGameState.generated.h"

class ARodnelpsPlayerState;
class APlayerBoardSpace;
class AInterpolationManager;
class AGameElementsGenerator;

UCLASS()
class RODNELPS_API ARodnelpsGameState : public AGameState
{
	GENERATED_BODY()

public:
	ARodnelpsGameState();

	ARodnelpsPlayerState* getActivePlayer();

	void addPlayer(APlayerBoardSpace* playerBoard);

	void setInterpolationManager(AInterpolationManager* interpolationManager);
	AInterpolationManager* GetInterpolationManager();

	void setGameElementGenerator(AGameElementsGenerator* gameElementsGenerator);

private:
	ARodnelpsPlayerState* m_ActivePlayer;
	TArray<ARodnelpsPlayerState*> m_PlayersArray;
	AInterpolationManager* m_InterpolManager;
	AGameElementsGenerator* m_GameElementsGenerator;
};

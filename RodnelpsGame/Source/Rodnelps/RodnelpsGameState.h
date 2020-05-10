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

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	void setInterpolationManager(AInterpolationManager* interpolationManager);
	AInterpolationManager* GetInterpolationManager();

	AGameElementsGenerator* getGameElementGenerator();
	void setGameElementGenerator(AGameElementsGenerator* gameElementsGenerator);

	void Server_setActivePlayer(ARodnelpsPlayerState* player);

private:

	UPROPERTY(replicated)
	ARodnelpsPlayerState* m_ActivePlayer;

	AInterpolationManager* m_InterpolManager;
	AGameElementsGenerator* m_GameElementsGenerator;
};

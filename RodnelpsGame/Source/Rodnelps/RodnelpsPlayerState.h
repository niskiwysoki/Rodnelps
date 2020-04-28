// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerBoardSpace.h"
#include "Token.h"
#include "OwnershipInterface.h"
#include "RodnelpsPlayerState.generated.h"

class ARodnelpsGameState;


UCLASS()
class RODNELPS_API ARodnelpsPlayerState : public APlayerState, public IOwnershipInterface
{
	GENERATED_BODY()

public:
	ARodnelpsPlayerState();

	void setPlayerBoard(APlayerBoardSpace* playerBoard);
	APlayerBoardSpace* getPlayerBoard();
	
	void addToken(AToken* token);
	void removeToken(AToken* token);

	void resetStatusAndEndTurn(ARodnelpsGameState* gamestate);

	int32 getTokenNum();

	bool areTokensDrawn();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
		bool isTaken();
	virtual bool isTaken_Implementation() override;

private:
	TArray<TArray<AToken*>> m_TokenStacksArray;

	ETokenColor m_FirstTokenTakenColor;
	ETokenColor m_SecondTokenTakenColor;
	bool m_AreTokensDrawn;
	TArray<bool> m_ReservedCards;

	APlayerBoardSpace* m_PlayerBoard;
};

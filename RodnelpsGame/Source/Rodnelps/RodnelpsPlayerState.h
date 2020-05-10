// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerBoardSpace.h"
#include "OwnershipInterface.h"
#include "Token.h"
#include "RodnelpsUtils.h"
#include "RodnelpsPlayerState.generated.h"

class ARodnelpsGameState;
class ACard;
class ATraderCard;

UCLASS()
class RODNELPS_API ARodnelpsPlayerState : public APlayerState, public IOwnershipInterface
{
	GENERATED_BODY()

public:
	ARodnelpsPlayerState();
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	void setPlayerBoard(APlayerBoardSpace* playerBoard);
	APlayerBoardSpace* getPlayerBoard();
	
	void addStandardToken(AToken* token);

	UFUNCTION(Server, WithValidation, Reliable)
	void addToken(AToken* token);

	UFUNCTION(Server, WithValidation, Reliable)
	void removeToken(AToken* token);

	void resetTokenStatusAndEndTurn(ARodnelpsGameState* gamestate);
	int32 getTokenNum();
	bool areTokensDrawn();
	bool isTakingTokens();
	bool isTakingTraders();
	void setIsTakingTraders(bool status);

	bool areCardRequirementsFulfilled(ACard* card);

	UFUNCTION(Server, WithValidation, Reliable)
	void addCard(ACard* card);

	bool isTraderPosibbleToGet();

	void transferTrader(ATraderCard* trader);
	bool isMeetsTraderRequirements(ATraderCard* trader);

	void payForCard(ACard* card);
	void reserveCard(ACard* card);

	void moveActorOnBoard(AActor* actor, FVector desiredLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
		bool isTaken();
	virtual bool isTaken_Implementation() override;

	virtual void setTokenIndex_Implementation(AToken* token) override;

	void setPlayerId(int32 id);
	int32 getPlayerId() const { return m_PlayerId; }

private:

	UFUNCTION(Server, WithValidation, Reliable)
	void endTurn();

private:

	UPROPERTY(Replicated)
	TArray<FTokenArray> m_TokenStacksArray;

	ETokenColor m_FirstTokenTakenColor;
	ETokenColor m_SecondTokenTakenColor;
	bool m_AreTokensDrawn;
	bool m_isTakingTokens;
	bool m_isTakingTrader;

	UPROPERTY(Replicated)
	TArray <FCardArray> m_CardStacksArray;

	UPROPERTY(Replicated)
	TArray <ACard*> m_ReservedCardArray;

	UPROPERTY(Replicated)
	TArray <ATraderCard*> m_TraderCardArray;

	void payTokenStackForCard(int32 tokensNum, int32 colorIndex);

	UPROPERTY(replicated)
	int32 m_PlayerId;

	UPROPERTY(replicated)
	APlayerBoardSpace* m_PlayerBoard;
};

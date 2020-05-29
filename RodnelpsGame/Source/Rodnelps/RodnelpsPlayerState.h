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
	void Server_removeToken(AToken* token);

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

	UFUNCTION(Server, WithValidation, Reliable)
	void transferTrader(ATraderCard* trader);

	bool isMeetsTraderRequirements(ATraderCard* trader);

	void payForCard(ACard* card);

	UFUNCTION(Server, WithValidation, Reliable)
	void reserveCard(ACard* card);

	void moveActorOnBoard(AActor* actor, FVector desiredLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
		bool isTaken();
	virtual bool isTaken_Implementation() override;

	virtual void setTokenIndex_Implementation(AToken* token) override;

	void setPlayerId(int32 id);
	int32 getPlayerId() const { return m_PlayerId; }

	TArray<FTokenArray> getTokenStacksArray() const { return m_TokenStacksArray; }
	TArray<FCardArray> getCardStacksArray() const { return m_CardStacksArray; }
	int32  getVictoryPoints() const { return m_VictoryPoints; }

	UFUNCTION(BlueprintImplementableEvent, Category = "GlobalEvents")
	void sendGuideMessage(const FString &message);

	UFUNCTION(NetMulticast, Reliable)
	void broadcast_SendGuideMessage(const FString &message);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GlobalEvents")
	void showMessageOnCenterOfScreen(const FString& message, float messageTime);

	UFUNCTION(NetMulticast, Reliable)
	void broadcast_showMessageOnCenterOfScreen(const FString& message, float messageTime);

private:
	UFUNCTION(Server, WithValidation, Reliable)
	void endTurn();

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 m_PlayerId;

private:
	ETokenColor m_FirstTokenTakenColor;
	ETokenColor m_SecondTokenTakenColor;
	bool m_AreTokensDrawn;
	bool m_isTakingTokens;

	int32 m_VictoryPoints;

	UPROPERTY(Replicated)
	bool m_isTakingTrader;

	UPROPERTY(Replicated)
	TArray<FTokenArray> m_TokenStacksArray;

	UPROPERTY(Replicated)
	TArray <FCardArray> m_CardStacksArray;

	UPROPERTY(Replicated)
	TArray <ACard*> m_ReservedCardArray;

	UPROPERTY(Replicated)
	TArray <ATraderCard*> m_TraderCardArray;

	void payTokenStackForCard(int32 tokensNum, int32 colorIndex);

	UPROPERTY(Replicated)
	APlayerBoardSpace* m_PlayerBoard;
};

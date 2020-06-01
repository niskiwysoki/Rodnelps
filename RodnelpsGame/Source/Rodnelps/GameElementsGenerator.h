// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OwnershipInterface.h"
#include "RodnelpsUtils.h"
#include "GameElementsGenerator.generated.h"

class UDataTable;
class ATraderCard;
class ACard;
class AToken;
class AInterpolationManager;
class UMaterial;

UCLASS()
class RODNELPS_API AGameElementsGenerator : public AActor, public IOwnershipInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameElementsGenerator();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	const TArray<ATraderCard*>& getTraderArray();

	void generateGamePieces(int32 numberOfPlayers);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LayOutTheCards();
	void generateTokens(int32 numberOfPlayers);
	void generateTraders(int32 numberOfPlayers, float distanceBetweenTraders);
	void generateDecks(float cardHeightDiffrence, float distanceBetweenDecks);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
		bool isTaken();	//interface
	virtual bool isTaken_Implementation() override;

	virtual void setTokenIndex_Implementation(AToken* token) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void addToken(AToken* token);
	void removeToken(AToken* token);
	int32 getStackSize(AToken* token);
	TArray<AToken*> getGoldTokenStack();
	TArray<AToken*> getTokenStack(AToken* token);
	void removeTrader(ATraderCard* trader);

	void placeNewCard(ACard* card);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CardTables", meta = (AllowPrivateAccess = "true"))
	TArray<UDataTable*> m_CardTablesArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TraderTables", meta = (AllowPrivateAccess = "true"))
	UDataTable* m_TraderCards;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CardTables", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACard> m_CardToSpawn;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TraderTables", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATraderCard> m_TraderToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Token", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AToken> m_TokenToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InterpolationManager", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AInterpolationManager> m_InterpolationManagerToSpawn;

	UPROPERTY(Replicated)
	TArray<FCardArray> m_DecksArray;

	UPROPERTY(Replicated)
	TArray<FTokenArray> m_TokenStacsArray;

	TArray<ATraderCard*> m_TradersArray;
};

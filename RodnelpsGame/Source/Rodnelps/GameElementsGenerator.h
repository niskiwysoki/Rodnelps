// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OwnershipInterface.h"
#include "GameElementsGenerator.generated.h"

class UDataTable;
class ATraderCard;
class ACard;
class AToken;
class AInterpolationManager;


UCLASS()
class RODNELPS_API AGameElementsGenerator : public AActor, public IOwnershipInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameElementsGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LayOutTheCards();
	void generateTokens();
	void generateTraders(float distanceBetweenTraders);
	void generateDecks(float cardHeightDiffrence, float distanceBetweenDecks);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
		bool isTaken();	//interface
	virtual bool isTaken_Implementation() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void addToken(AToken* token);
	void removeToken(AToken* token);
	int32 getStackSize(AToken* token);
	TArray<AToken*> getGoldTokenStack();

	TArray<ATraderCard*> getTraderArray();
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
	
	TArray<TArray<ACard*>> m_DecksArray;

	TArray<TArray<AToken*>> m_TokenStacsArray;

	TArray<ATraderCard*> m_TradersArray;
};

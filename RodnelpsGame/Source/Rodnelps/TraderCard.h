// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "TraderCard.generated.h"


USTRUCT(BlueprintType)
struct RODNELPS_API FTraderSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardInfo")
		int32 ReqWhite = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardInfo")
		int32 ReqBlue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardInfo")
		int32 ReqGreen = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardInfo")
		int32 ReqRed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardInfo")
		int32 ReqBlack = 0;
};

UCLASS()
class RODNELPS_API ATraderCard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATraderCard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSelected(AActor* Target, FKey ButtonPressed);

	void SetTraderInfo(FTraderSettings* TraderInfo);

	FTraderSettings* getTraderInfo();

	bool isTaken();
	void setIsTaken(bool status);

private:
	FTraderSettings* m_TraderSettings;
	bool m_isTaken;


};

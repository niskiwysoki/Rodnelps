// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TraderUserWidget.generated.h"

class ATraderCard;

/**
 * 
 */
UCLASS()
class RODNELPS_API UTraderUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UTraderUserWidget(const FObjectInitializer& init);

	void setTrader(ATraderCard* trader);

	UFUNCTION(BlueprintImplementableEvent, Category = "Data")
	void OnTraderSet(ATraderCard* trader);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	ATraderCard* m_Trader;
};

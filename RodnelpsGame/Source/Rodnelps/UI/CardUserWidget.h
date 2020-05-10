// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardUserWidget.generated.h"

class ACard;
class UTextBlock;

UCLASS()
class RODNELPS_API UCardUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UCardUserWidget(const FObjectInitializer& init);

	void setCard(ACard* card);

	void NotifyCardDataChanged();

	UFUNCTION(BlueprintImplementableEvent, Category = "Data")
	void OnCardSet(ACard* card);

protected:

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* m_VictoryPointsText;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	ACard* m_Card;
	
};

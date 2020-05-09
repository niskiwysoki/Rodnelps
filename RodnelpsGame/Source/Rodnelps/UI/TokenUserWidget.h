// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Token.h"
#include "TokenUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class RODNELPS_API UTokenUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UTokenUserWidget(const FObjectInitializer& init);

	void setToken(AToken* token);

	UFUNCTION(BlueprintImplementableEvent, Category = "Data")
	void OnTokenSet(AToken* token);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	AToken* m_Token;
};

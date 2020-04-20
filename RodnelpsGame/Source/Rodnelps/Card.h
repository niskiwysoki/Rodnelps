// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Card.generated.h"

UENUM(BlueprintType)
enum class ECardColor : uint8
{
	WHITE UMETA(DisplayName = "WHITE"), 
	BLUE  UMETA(DisplayName = "BLUE"), 
	GREEN  UMETA(DisplayName = "GREEN"), 
	RED  UMETA(DisplayName = "RED"),
	BLACK  UMETA(DisplayName = "BLACK"),
	MAX_COLORS UMETA(DisplayName = "NumOfColors"),
};

USTRUCT(BlueprintType)
struct RODNELPS_API FCardSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardInfo")
	int32 VictoryPoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardInfo")
	ECardColor CardColor = ECardColor::WHITE;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardInfo")
	int32 CardTier = 1;
};

UCLASS()
class RODNELPS_API ACard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void onSelected(AActor* Target, FKey ButtonPressed);

	void setCardInfo(FCardSettings* CardInfo);

private:
	FCardSettings* m_CardSettings;

	//FVector m_DesiredLocation;
	//FRotator m_DesiredRotation;
	//float m_DesiredLocRotDelay;
};

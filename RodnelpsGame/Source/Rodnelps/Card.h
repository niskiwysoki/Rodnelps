// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Card.generated.h"

class UWidgetComponent;
class UStaticMeshComponent;
class UMaterial;

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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CardTables", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* m_WidgetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CardTables", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_Mesh;

public:	
	// Sets default values for this actor's properties
	ACard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void logOutCardInfo();

public:	

	UFUNCTION()
	void onSelected(AActor* Target, FKey ButtonPressed);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	void setCardInfo(FCardSettings* CardInfo);

	void setMaterial(int materialIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FCardSettings& GetCardInfoBP() const { return m_CardSettings;}

	FCardSettings* getCardInfo();
	
	bool isTaken();
	void setAsTaken();
	bool isInDeck();
	void setAsNotInDeck();
	bool isReserved();
	void setIsReservedStatus(bool status);
	bool isOnTopOfDeck();
	void setIsOnTopOfDeck(bool status);

private:

	UFUNCTION()
	void onCardSettingsChanged();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Card", meta = (AllowPrivateAccess = "true"))
	TArray<UMaterialInstance*> m_CardMaterialsArray;

	UPROPERTY(ReplicatedUsing = onCardSettingsChanged)
	FCardSettings m_CardSettings;

	UPROPERTY(Replicated)
	bool m_isTaken;

	UPROPERTY(Replicated)
	bool m_isReserved;

	UPROPERTY(Replicated)
	bool m_IsInDeck;

	UPROPERTY(Replicated)
	bool m_IsOnTopOfDeck;
};

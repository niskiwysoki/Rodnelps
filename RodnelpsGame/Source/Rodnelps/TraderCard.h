// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "TraderCard.generated.h"

class UWidgetComponent;


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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CardTables", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* m_WidgetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CardTables", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_Mesh;


public:	
	// Sets default values for this actor's properties
	ATraderCard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSelected(AActor* Target, FKey ButtonPressed);

	void SetTraderInfo(FTraderSettings* TraderInfo);

	void SyncWidgetData();

	FTraderSettings* getTraderInfo();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FTraderSettings& GetTraderInfoBP() const { return m_TraderSettings; }

	bool isTaken();
	void setIsTaken(bool status);

	UFUNCTION()
	void onTraderSettingsChanged();

private:

	UPROPERTY(ReplicatedUsing = onTraderSettingsChanged)
	FTraderSettings m_TraderSettings;

	UPROPERTY(Replicated)
	bool m_isTaken;

};

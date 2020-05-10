// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Token.generated.h"

class IOwnershipInterface;
class UWidgetComponent;

UENUM(BlueprintType)
enum class ETokenColor : uint8
{
	WHITE,
	BLUE,
	GREEN,
	RED,
	BLACK,
	GOLD,
	MAX_COLOURS
};


UCLASS()
class RODNELPS_API AToken : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Token", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Token", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* m_WidgetComp;

public:	
	// Sets default values for this actor's properties
	AToken();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSelected(AActor* Target, FKey ButtonPressed);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void setColor(ETokenColor tokenColor);
	ETokenColor getColor();

	void setMaterial(UMaterial* material);

	void setOwner(UObject* newOwner);

	void SetTokenUI();

	UFUNCTION(NetMulticast, Reliable)
	void Broadcast_SetTokenUI();

	void setTokenIndex(int32 index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const int32& GetTokenIndexBP() const { return m_TokenIndex; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const ETokenColor& getTokenColorBP() const { return m_Color; }

private:

	UFUNCTION()
	void onColorRep();

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Token", meta = (AllowPrivateAccess = "true"))
	TArray<UMaterial*> m_TokenMaterialsArray;

	UPROPERTY(ReplicatedUsing = onColorRep)
	ETokenColor m_Color;
	
	UPROPERTY(Replicated)
	UObject* m_Owner;

	UPROPERTY(Replicated)
	int32 m_TokenIndex;
	
};

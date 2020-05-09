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

	void setTokenIndex(int32 index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const int32& GetTokenIndexBP() const { return m_TokenIndex; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const ETokenColor& getTokenColorBP() const { return m_Color; }

private:
	ETokenColor m_Color;
	
	UObject* m_Owner;
	int32 m_TokenIndex;
	
};

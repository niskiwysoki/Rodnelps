// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UBoxComponent;
class UCameraComponent;

UCLASS()
class RODNELPS_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void caclculatePlayerHeight();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input functions
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);
	void Move_ZAxis(float AxisValue);

private:
	// Input variables
	FVector CurrentVelocity;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	int m_cameraMovementSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision Box", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	float m_playerHeight;

};

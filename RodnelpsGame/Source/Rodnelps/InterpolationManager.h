// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h"
#include "InterpolationManager.generated.h"

UCLASS()
class RODNELPS_API AInterpolationManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInterpolationManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void setDesiredRotation(AActor* interpolatedActor, const FRotator& NewRot, float delay);
	void setDesiredLocation(AActor* interpolatedActor, const FVector& NewLoc, float delay);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	float m_InterpolationSpeed = 15;

private:
	struct SInterpolData {
		AActor* interpolatedActor;
		FVector m_DesiredLocation = FVector::ZeroVector;
		FRotator m_DesiredRotation = FRotator(45.f,45.f,45.f);
		float m_DesiredLocRotDelay = 0.f;
	};
	TQueue<SInterpolData*> m_InterpolationQueue;

};

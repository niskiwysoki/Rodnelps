// Fill out your copyright notice in the Description page of Project Settings.


#include "InterpolationManager.h"
#include "RodnelpsGameState.h"


// Sets default values
AInterpolationManager::AInterpolationManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInterpolationManager::BeginPlay()
{
	Super::BeginPlay();
	ARodnelpsGameState* gameState = GetWorld()->GetGameState<ARodnelpsGameState>();
	gameState->setInterpolationManager(this);
}

void AInterpolationManager::setDesiredRotation(AActor* interpolatedActor,const FRotator& newRot, float delay)
{
	SInterpolData* rotRequest = new SInterpolData;
	rotRequest->interpolatedActor = interpolatedActor;
	rotRequest->m_DesiredRotation = newRot;
	rotRequest->m_DesiredLocRotDelay = delay;
	m_InterpolationQueue.Enqueue(rotRequest);

	PrimaryActorTick.SetTickFunctionEnable(true);
}

void AInterpolationManager::setDesiredLocation(AActor* interpolatedActor, const FVector& newLoc, float delay)
{
	SInterpolData* translationRequest = new SInterpolData;
	translationRequest->interpolatedActor = interpolatedActor;
	translationRequest->m_DesiredLocation = newLoc;
	translationRequest->m_DesiredLocRotDelay = delay;
	m_InterpolationQueue.Enqueue(translationRequest);

	PrimaryActorTick.SetTickFunctionEnable(true);
}


// Called every frame
void AInterpolationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_InterpolationQueue.Peek())
	{
		SInterpolData* ReqInterpolation = *m_InterpolationQueue.Peek();

		if (ReqInterpolation)
		{
			if (ReqInterpolation->m_DesiredLocRotDelay > 0.f)
			{
				ReqInterpolation->m_DesiredLocRotDelay -= DeltaTime;
				return;
			}

			if (ReqInterpolation->m_DesiredLocation != FVector::ZeroVector)
			{
				FVector Loc = FMath::VInterpTo(ReqInterpolation->interpolatedActor->GetActorLocation(),
					ReqInterpolation->m_DesiredLocation, DeltaTime, m_InterpolationSpeed);

				if (Loc != ReqInterpolation->interpolatedActor->GetActorLocation())
				{
					ReqInterpolation->interpolatedActor->SetActorLocation(Loc);
				}
				else
				{
					m_InterpolationQueue.Pop();
					return;
				}

			}

			if (ReqInterpolation->m_DesiredRotation != FRotator(45.f, 45.f, 45.f))
			{
				FRotator Rot = FMath::RInterpTo(ReqInterpolation->interpolatedActor->GetActorRotation(),
					ReqInterpolation->m_DesiredRotation, DeltaTime, m_InterpolationSpeed);
				if (!(Rot - ReqInterpolation->interpolatedActor->GetActorRotation()).IsNearlyZero(0.01f))
				{
					ReqInterpolation->interpolatedActor->SetActorRotation(Rot);
				}
				else
				{
					ReqInterpolation->interpolatedActor->SetActorRotation(ReqInterpolation->m_DesiredRotation);
					m_InterpolationQueue.Pop();
					return;
				}
			}
		}

		if (m_InterpolationQueue.IsEmpty())
		{
			PrimaryActorTick.SetTickFunctionEnable(false);
		}
	}
	
}


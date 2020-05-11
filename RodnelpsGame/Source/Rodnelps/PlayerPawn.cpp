// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	VisibleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = VisibleComponent;
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeRotation(FRotator(-75.0f, 0.0f, 0.0f));
	CameraMovementSpeed = 800;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle movement based on our "MoveX" and "MoveY" axes

	if (!CurrentVelocity.IsZero())
	{
		FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
		SetActorLocation(NewLocation);
		CurrentVelocity = FVector::ZeroVector;
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("MoveX", this, &APlayerPawn::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &APlayerPawn::Move_YAxis);
	InputComponent->BindAxis("MoveZ", this, &APlayerPawn::Move_ZAxis);
}

void APlayerPawn::Move_XAxis(float AxisValue)
{
	// Move at 100 units per second forward or backward
	CurrentVelocity += GetActorRotation().RotateVector(FVector(FMath::Clamp(AxisValue, -1.0f, 1.0f) * CameraMovementSpeed, 0.f, 0.f));
}

void APlayerPawn::Move_YAxis(float AxisValue)
{
	// Move at 100 units per second right or left
	CurrentVelocity += GetActorRotation().RotateVector(FVector(0.f, FMath::Clamp(AxisValue, -1.0f, 1.0f) * CameraMovementSpeed, 0.f));
}

void APlayerPawn::Move_ZAxis(float AxisValue)
{
	// Move at 100 units per second forward or backward
	CurrentVelocity += GetActorRotation().RotateVector(FVector(0.f, 0.f,FMath::Clamp(AxisValue, -1.0f, 1.0f) * CameraMovementSpeed * 3));
}

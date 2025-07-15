// Fill out your copyright notice in the Description page of Project Settings.


#include "Wolf.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

// Sets default values
AWolf::AWolf()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->MaxSpeed = 600.0f;

	// Lets it use AddMovementInput 
	//AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AWolf::BeginPlay()
{
	Super::BeginPlay();
	target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AWolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target != nullptr)
	{
		// Distance from player
		FVector distance = target->GetActorLocation() - GetActorLocation();

		while (distance.X <= 100.0f && distance.Z <= 25.0f)
		{
			// Gets direction to player and then moves towards it
			FVector direction = (distance).GetSafeNormal();
			AddMovementInput(direction, 1.0f);
		}
	}
}

//// Called to bind functionality to input
//void AWolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}


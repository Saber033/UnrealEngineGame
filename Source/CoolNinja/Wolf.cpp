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

	// Capsule component for collision
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CollisionComponent->InitCapsuleSize(42.0f, 96.0f);

	RootComponent = CollisionComponent;

	// Sprite component
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetupAttachment(CollisionComponent);

	static ConstructorHelpers::FObjectFinder<UPaperSprite> SpriteAsset(TEXT("/Game/2DSideScroller/KnightAssets/KnightIdle_Sprite_1.KnightIdle_Sprite_1"));
	if (SpriteAsset.Succeeded())
	{
		SpriteComponent->SetSprite(SpriteAsset.Object);
	}

	//intializing other data members
	dead = false;
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

	if (target != nullptr && !dead)
	{
		// Distance from player
		FVector distance;
		distance.X = std::abs(target->GetActorLocation().X - GetActorLocation().X);
		distance.Z = std::abs(target->GetActorLocation().Z - GetActorLocation().Z);

		int temp = 0;

		while (distance.X <= 1300.0f && distance.Z <= 500.0f && temp < 1000)
		{
			// Gets direction to player and then moves towards it
			FVector direction = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			AddMovementInput(direction, 1.0f);
			temp++;
		}
	}
}

//// Called to bind functionality to input
//void AWolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Wolf.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "CoolNinjaCharacter.h"

// Sets default values
AWolf::AWolf()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	// Lets it use AddMovementInput 
	//AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);

	// Sprite component
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetupAttachment(GetCapsuleComponent());

	static ConstructorHelpers::FObjectFinder<UPaperSprite> SpriteAsset(TEXT("/Game/2DSideScroller/KnightAssets/KnightIdle_Sprite_1.KnightIdle_Sprite_1"));
	if (SpriteAsset.Succeeded())
	{
		SpriteComponent->SetSprite(SpriteAsset.Object);
	}

	//intializing other data members
	dead = false;
	health = 5;
}

// Called when the game starts or when spawned
void AWolf::BeginPlay()
{
	Super::BeginPlay();
	Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AWolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (health <= 0)
	{
		dead = true;
	}

	if (dead)
	{
		SpriteComponent->SetVisibility(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (Target != nullptr && !dead)
	{
		// Distance from player
		FVector distance;
		distance.X = std::abs(Target->GetActorLocation().X - GetActorLocation().X);
		distance.Z = std::abs(Target->GetActorLocation().Z - GetActorLocation().Z);

		int temp = 0;

		while (distance.X <= 1300.0f && temp < 1000)
		{
			// Gets direction to player and then moves towards it
			FVector direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			AddMovementInput(direction, 1.0f);
			temp++;

			if (distance.Z <= 500.0f && distance.Z >= 100.0f)
			{
				Jump();
			}

			if (GetCapsuleComponent()->IsOverlappingActor(Target))
			{
				// Attack player
				Cast<ACoolNinjaCharacter>(Target)->Damage(1.0f);
			}
		}
	}
}

//// Called to bind functionality to input
//void AWolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}


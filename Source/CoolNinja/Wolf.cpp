// Fill out your copyright notice in the Description page of Project Settings.


#include "Wolf.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

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
	damage_cooldown = 1.0f;
	damage_timer = 0.0f;
}

// Called when the game starts or when spawned
void AWolf::BeginPlay()
{
	Super::BeginPlay();
	Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called every frame
void AWolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	damage_timer -= DeltaTime;

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
		float distance_to_player = FVector::Dist(Target->GetActorLocation(), GetActorLocation());

		FVector distance;
		distance.X = std::abs(Target->GetActorLocation().X - GetActorLocation().X);
		distance.Z = std::abs(Target->GetActorLocation().Z - GetActorLocation().Z);

		if (distance.X <= 1300.0f)
		{
			// Gets direction to player and then moves towards it
			FVector direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			AddMovementInput(direction, 1.0f);

			if (distance.Z <= 500.0f && distance.Z >= 100.0f)
			{
				Jump();
			}

			if (distance_to_player < 100.0f && damage_timer <= 0.0f)
			{
				UGameplayStatics::ApplyDamage(Target, 10.0f, GetController(), this, nullptr);
				
				damage_timer = damage_cooldown;
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


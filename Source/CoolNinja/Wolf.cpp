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
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> WolfFlipbook(TEXT("/Game/2DSideScrollerCPP/Sprites/WolfFlipbook.WolfFlipbook"));

	if (WolfFlipbook.Succeeded())
	{
		GetSprite()->SetFlipbook(WolfFlipbook.Object);

		GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, -104.0f));

		GetSprite()->SetUsingAbsoluteRotation(false);
		GetSprite()->SetRelativeRotation(FRotator::ZeroRotator);
	}

	//intializing other data members
	dead = false;
	health = 5;
	damage_cooldown = 1.0f;
	damage_timer = 0.0f;

	GetCharacterMovement()->JumpZVelocity = 750.0f;
}

// Called when the game starts or when spawned
void AWolf::BeginPlay()
{
	Super::BeginPlay();

	GetSprite()->SetRelativeScale3D(FVector(2.0f));

	Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

float AWolf::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	health -= DamageAmount;

	if (health <= 0)
	{
		dead = true;
	}

	return DamageAmount;
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
		GetSprite()->SetVisibility(false);
		GetSprite()->SetHiddenInGame(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (Target != nullptr && !dead)
	{
		// Distance from player
		float distance_to_player = FVector::Dist(Target->GetActorLocation(), GetActorLocation());

		FVector distance;
		distance.X = std::abs(Target->GetActorLocation().X - GetActorLocation().X);
		distance.Z = std::abs(Target->GetActorLocation().Z - GetActorLocation().Z);

		if (distance.X <= 2400.0f)
		{
			// Gets direction to player and then moves towards it
			FVector direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			AddMovementInput(direction, 1.0f);

			if (distance.Z <= 500.0f && distance.Z >= 100.0f && direction.Z >= 0)
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


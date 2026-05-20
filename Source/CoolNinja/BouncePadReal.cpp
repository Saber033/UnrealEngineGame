// Fill out your copyright notice in the Description page of Project Settings.


#include "BouncePadReal.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABouncePadReal::ABouncePadReal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	hitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("hitbox"));
	RootComponent = hitbox;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetupAttachment(hitbox);

}

// Called when the game starts or when spawned
void ABouncePadReal::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

}

// Called every frame
void ABouncePadReal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Distance from player
	float distance = FVector::Dist(Player->GetActorLocation(), GetActorLocation());
	if ((distance * distance < radius * radius) && (cooldown == 0)) {
		FVector launchPattern;
		launchPattern.Y = strength;
		Player->LaunchCharacter(launchPattern, false, false);
		cooldown = maxCooldown;
	}
	if (cooldown > 0) {
		cooldown -= DeltaTime;
	}
	if (cooldown < 0) { cooldown = 0; }

}


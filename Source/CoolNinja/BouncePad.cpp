// Fill out your copyright notice in the Description page of Project Settings.


#include "BouncePad.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABouncePad::ABouncePad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	radius = 200.0f;
	maxCooldown = 50.0f;

}

// Called when the game starts or when spawned
void ABouncePad::BeginPlay()
{
	Super::BeginPlay();
	target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	cooldown = 0;
}

// Called every frame
void ABouncePad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (target != nullptr)
	{
		// Distance from player
		float distance = 0.0f;
		distance += (std::abs(target->GetActorLocation().X - GetActorLocation().X)) * (std::abs(target->GetActorLocation().X - GetActorLocation().X));
		distance += (std::abs(target->GetActorLocation().Z - GetActorLocation().Z)) * (std::abs(target->GetActorLocation().Z - GetActorLocation().Z));
		if (distance < radius * radius)
		{
			if (cooldown < maxCooldown)
			{
				ACharacter* character = Cast<ACharacter>(target);
				character->LaunchCharacter(FVector(0, 0, 3000), true, true);
				cooldown = 0;
			}
			else
			{
				cooldown++;
			}
		}
	}

}


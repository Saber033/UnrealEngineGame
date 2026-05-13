// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "Wolf.generated.h"

UCLASS()
class COOLNINJA_API AWolf : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWolf();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	/*virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(EditAnywhere)
	AActor* Target;

	bool dead;
};

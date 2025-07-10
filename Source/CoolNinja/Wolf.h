// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Wolf.generated.h"

UCLASS()
class COOLNINJA_API AWolf : public APawn
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(EditAnywhere)
	AActor* target;
};

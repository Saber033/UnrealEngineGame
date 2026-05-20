// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoolNinjaCharacter.h"
#include "PaperSpriteComponent.h"
#include "Components/CapsuleComponent.h"
#include "BouncePadReal.generated.h"

UCLASS()
class COOLNINJA_API ABouncePadReal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABouncePadReal();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(EditAnywhere)
	float strength = 1000.0f;
	const float radius = 50.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UCapsuleComponent* hitbox;

private:
	ACharacter* Player;
	const float maxCooldown = 60;
	float cooldown = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

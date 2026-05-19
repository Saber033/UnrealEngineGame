

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "CoolNinjaCharacter.h"
#include "Wolf.generated.h"

UCLASS()
class COOLNINJA_API AWolf : public ACharacter
{
	GENERATED_BODY()

public:

	AWolf();

	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(EditAnywhere)
	ACharacter* Target;

private:
	bool dead;
	int health;
	float damage_cooldown;
	float damage_timer;
};

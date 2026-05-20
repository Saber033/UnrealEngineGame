

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "CoolNinjaCharacter.h"
#include "Wolf.generated.h"

UCLASS()
class COOLNINJA_API AWolf : public APaperCharacter
{
	GENERATED_BODY()

public:

	AWolf();

	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

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

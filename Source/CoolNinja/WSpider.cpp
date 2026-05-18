#include "Spider.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "CoolNinjaCharacter.h"

ASpider::ASpider()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(21.0f, 48.0f);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetupAttachment(GetCapsuleComponent());

	static ConstructorHelpers::FObjectFinder<UPaperSprite> SpriteAsset(TEXT("/Game/2DSideScroller/SpiderAssets/SpiderIdle_Sprite.SpiderIdle_Sprite"));
	if (SpriteAsset.Succeeded())
	{
		SpriteComponent->SetSprite(SpriteAsset.Object);
	}

	dead = false;
	health = 2;
}

void ASpider::BeginPlay()
{
	Super::BeginPlay();
	Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ASpider::Tick(float DeltaTime)
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
		FVector distance;
		distance.X = std::abs(Target->GetActorLocation().X - GetActorLocation().X);
		distance.Z = std::abs(Target->GetActorLocation().Z - GetActorLocation().Z);

		int temp = 0;

		while (distance.X <= 1300.0f && distance.Z <= 500.0f && temp < 1000)
		{
			FVector direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			AddMovementInput(direction, 1.0f);
			temp++;

			if (distance.X == 0.0f && distance.Z == 0.0f)
			{
				Cast<ACoolNinjaCharacter>(Target)->Damage(0.5f);
			}
		}
	}
}

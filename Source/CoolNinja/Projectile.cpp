// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;

    CollisionComponent->InitSphereRadius(1.0f);


    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);

    CollisionComponent->SetNotifyRigidBodyCollision(true);

    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnProjectileHit);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    Mesh->SetupAttachment(RootComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

    SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
    SpriteComponent->SetupAttachment(CollisionComponent);
    SpriteComponent->SetRelativeScale3D(FVector(0.3f));

    static ConstructorHelpers::FObjectFinder<UPaperSprite> SpriteAsset(TEXT("/Game/2DSideScrollerCPP/Sprites/shuriken_sprite_Sprite.shuriken_sprite_Sprite"));
    if (SpriteAsset.Succeeded())
    {
        SpriteComponent->SetSprite(SpriteAsset.Object);
		SpriteComponent->SetRelativeLocation(FVector(-50.0f, 0.0f, -90.0f));
		SpriteComponent->SetUsingAbsoluteRotation(true);
    }

    ProjectileMovement->InitialSpeed = 2000.0f;
    ProjectileMovement->MaxSpeed = 2000.0f;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Initialize(FVector Direction)
{
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
}

void AProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
        if (OtherActor == GetOwner())
        {
            return;
        }
        
        UGameplayStatics::ApplyDamage(OtherActor, 1.0f, GetInstigatorController(), this, nullptr);

		Destroy();
	}
}
;



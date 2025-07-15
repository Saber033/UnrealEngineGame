// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoolNinjaCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// ACoolNinjaCharacter

ACoolNinjaCharacter::ACoolNinjaCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;
}

void ACoolNinjaCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->GravityScale = 3.5f;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->JumpZVelocity = 1800.0f;
	GetCharacterMovement()->GroundFriction = 0.0f;
	GetCharacterMovement()->MaxWalkSpeed = 3000.0f;
	GetCharacterMovement()->MaxAcceleration = 10000.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 8000.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	DashSpeed = 3000.0f;
	DashTimer = 0.0f;
}



//////////////////////////////////////////////////////////////////////////
// Animation

void ACoolNinjaCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	UPaperFlipbook* CurrentAnimation = GetSprite()->GetFlipbook();
	if( CurrentAnimation != DesiredAnimation && (CurrentAnimation == IdleAnimation || CurrentAnimation == RunningAnimation)	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void ACoolNinjaCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter(DeltaSeconds);	
}


//////////////////////////////////////////////////////////////////////////
// Input

void ACoolNinjaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACoolNinjaCharacter::MoveRight);
	
	// added
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ACoolNinjaCharacter::Dash);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACoolNinjaCharacter::Attack);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &ACoolNinjaCharacter::Throw);


	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACoolNinjaCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACoolNinjaCharacter::TouchStopped);
}

void ACoolNinjaCharacter::MoveRight(float Value)
{
	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void ACoolNinjaCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void ACoolNinjaCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void ACoolNinjaCharacter::Dash()
{
	GetSprite()->SetFlipbook(DashAnimation);
	// pause player inputs?
	// pause player velocity
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	GetCharacterMovement()->StopMovementImmediately();

	bool bFacingRight = GetControlRotation().Yaw == 0.0f;
	GetCharacterMovement()->Velocity.X = bFacingRight ? DashSpeed : -1 * DashSpeed;
	GetCharacterMovement()->GravityScale = 0.0f;
	DashTimer = 0.25f;
	bDashing = true;
	// set it to somethig based on the dash
	// set velocity to 0
	// unpause player inputs?
}

void ACoolNinjaCharacter::Attack()
{
	GetSprite()->SetFlipbook(AttackAnimation);
}

void ACoolNinjaCharacter::Throw()
{
	GetSprite()->SetFlipbook(ThrowAnimation);
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(Shuriken, GetActorLocation(), GetActorRotation());
	if (Projectile)
	{
		Projectile->Initialize(GetActorForwardVector());
	}
}

void ACoolNinjaCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
	UE_LOG(LogTemp, Log, TEXT("jumped!"));
	// play jump animation
	if (JumpAnimation)
	{
		GetSprite()->SetPlayRate(1.2f);
		GetSprite()->SetFlipbook(JumpAnimation);
		GetSprite()->SetPlayRate(1.0f);
	}
}

void ACoolNinjaCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	UE_LOG(LogTemp, Log, TEXT("landed!"));
	GetSprite()->SetFlipbook(RunningAnimation);
}

void ACoolNinjaCharacter::UpdateCharacter(float DeltaSeconds)
{
	// Update animation to match the motion
	UpdateAnimation();

	if (DashTimer > 0.0f)
	{
		DashTimer -= DeltaSeconds;
	}
	else if (bDashing)
	{
		GetCharacterMovement()->GravityScale = 3.5f;
		GetSprite()->SetFlipbook(IdleAnimation);
		bDashing = false;
	}

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

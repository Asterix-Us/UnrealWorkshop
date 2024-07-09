// Fill out your copyright notice in the Description page of Project Settings.


#include "UWCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "UWInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


AUWCharacter::AUWCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(CameraBoom);

	InteractComp = CreateDefaultSubobject<UUWInteractionComponent>("InteractComp");

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AUWCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Test Rotation
	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called to bind functionality to input
void AUWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	const ULocalPlayer* LP = PC->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	// Add mappings for our game
	Subsystem->AddMappingContext(DefaultInputMapping, 0);

	UEnhancedInputComponent* InputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// General
	InputComp->BindAction(Input_Move, ETriggerEvent::Triggered, this, &AUWCharacter::Move);
	InputComp->BindAction(Input_Look, ETriggerEvent::Triggered, this, &AUWCharacter::Look);

	// Attack
	InputComp->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &AUWCharacter::PrimaryAttack);

	// Interact
	InputComp->BindAction(Input_Interact, ETriggerEvent::Triggered, this, &AUWCharacter::PrimaryInteract);
}

void AUWCharacter::Move(const FInputActionValue& InputValue)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;
	
	// Get value from input and convert to Vector(x,y)
	const FVector2D AxisValue = InputValue.Get<FVector2D>();

	// Move forward/back
	AddMovementInput(ControlRot.Vector(), AxisValue.Y);

	// Move right/left strafe
	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, AxisValue.X);
}

void AUWCharacter::Look(const FInputActionValue& InputValue)
{
	// Get value from input and convert to Vector(x,y)
	const FVector2D AxisValue = InputValue.Get<FVector2D>();
	
	AddControllerPitchInput(AxisValue.Y);
	AddControllerYawInput(AxisValue.X);
}

void AUWCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	// TimerHandle -> struct, reference to current timer
	// object -> us (character)
	// function -> to call after timer
	// delay -> timer duration
	GetWorldTimerManager().SetTimer(
		TimerHandle_PrimaryAttack,
		this,
		&AUWCharacter::PrimaryAttack_TimeElapsed,
		TimerDelay_PrimaryAttack);
}

void AUWCharacter::PrimaryAttack_TimeElapsed()
{
	const FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnTransform = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
}

void AUWCharacter::PrimaryInteract()
{
	if (InteractComp)
	{
		InteractComp->Interact();
	}
}

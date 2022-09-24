// Fill out your copyright notice in the Description page of Project Settings.


#include "JJBaseCharacter.h"

#include "JetPackJoyride3DGameModeBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"


// Sets default values
AJJBaseCharacter::AJJBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void AJJBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJJBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AJJBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(InputActions.MoveForward, ETriggerEvent::Triggered, this, &AJJBaseCharacter::MoveForward);
		EnhancedInput->BindAction(InputActions.MoveRight, ETriggerEvent::Triggered, this, &AJJBaseCharacter::MoveRight);
		EnhancedInput->BindAction(InputActions.TurnRight, ETriggerEvent::Triggered, this, &AJJBaseCharacter::TurnRight);
		
		// EnhancedInput->BindAction("IA_MoveForwardCC", this, &AJJBaseCharacter::MoveForward);
		// PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AJJBaseCharacter::MoveForwardOld);
	};
}

void AJJBaseCharacter::TurnRight(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.GetMagnitude());
}

void AJJBaseCharacter::MoveForward(const FInputActionValue& Value)
{
	const float ActionValue = Value.GetMagnitude();
	if ((Controller != nullptr) && (ActionValue != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, ActionValue);
	}
}

void AJJBaseCharacter::MoveRight(const FInputActionValue& Value)
{
	const float ActionValue = Value.GetMagnitude();
	if ((Controller != nullptr) && (ActionValue != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, ActionValue);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "JJBaseCharacter.h"

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
	CameraBoom->TargetArmLength = 400.0f;       // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AJJBaseCharacter::SetupStates()
{
	UE_LOG(LogTemp, Warning, TEXT("Initializing States"));
	AllStates.IdleState = NewObject<UIdleState>()->Init(this);
	AllStates.MovingState = NewObject<UMovingState>()->Init(this);
	AllStates.JumpingState = NewObject<UJumpingState>()->Init(this);
	AllStates.JetpackActiveState = NewObject<UJetpackState>()->Init(this);
	AllStates.InteractingState = NewObject<UInteractingState>()->Init(this);

	CurrentState = AllStates.IdleState;
	CurrentState->BeginState();
}

void AJJBaseCharacter::GoToState(UJJPlayerState* NewState)
{
	CurrentState->EndState();
	CurrentState = NewState;
	CurrentState->BeginState();
}

// Called when the game starts or when spawned
void AJJBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetupStates();
}

// Called every frame
void AJJBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(UJJPlayerState* NewState = CurrentState->Tick(DeltaTime); NewState != nullptr)
	{
		GoToState(NewState);
	}
}

// Called to bind functionality to input
void AJJBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(InputActions.MoveForward, ETriggerEvent::Triggered, this, &AJJBaseCharacter::HandleInput_MoveForward);
		EnhancedInput->BindAction(InputActions.MoveRight, ETriggerEvent::Triggered, this, &AJJBaseCharacter::HandleInput_MoveRight);
		EnhancedInput->BindAction(InputActions.TurnRight, ETriggerEvent::Triggered, this, &AJJBaseCharacter::HandleInput_Turn);
		EnhancedInput->BindAction(InputActions.Jump, ETriggerEvent::Started, this, &AJJBaseCharacter::HandleInput_Jump);
		EnhancedInput->BindAction(InputActions.Jetpack, ETriggerEvent::Started, this, &AJJBaseCharacter::HandleInput_Jetpack);
		EnhancedInput->BindAction(InputActions.Jetpack, ETriggerEvent::Completed, this, &AJJBaseCharacter::HandleInput_Jetpack);
		EnhancedInput->BindAction(InputActions.Interact, ETriggerEvent::Completed, this, &AJJBaseCharacter::HandleInput_Interact);
	};
}

void AJJBaseCharacter::HandleInput(UInputAction* InputAction, const FInputActionValue& Value)
{
	if(InputAction == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Input Action is Nullptr"));
		return;
	}
	// UE_LOG(LogTemp, Display, TEXT("Handle Input Called On Current State"));
	UJJPlayerState* NextState = CurrentState->HandleInput(InputAction, Value);
	if(NextState != nullptr)
	{
		GoToState(NextState);
	}
}

void AJJBaseCharacter::Interact()
{
	IsInteracting = true;
	PlayAnimMontage(InteractionMontage);
	GetWorldTimerManager().SetTimer(InteractionCompleteHandle, this, &AJJBaseCharacter::CompleteInteraction, 3, false);
}

void AJJBaseCharacter::CompleteInteraction()
{
	IsInteracting = false;
}

void AJJBaseCharacter::HandleInput_MoveForward(const FInputActionValue& Value)
{
	HandleInput(InputActions.MoveForward, Value);
}
void AJJBaseCharacter::HandleInput_MoveRight(const FInputActionValue& Value)
{
	HandleInput(InputActions.MoveRight, Value);
}
void AJJBaseCharacter::HandleInput_Turn(const FInputActionValue& Value)
{
	HandleInput(InputActions.TurnRight, Value);
}
void AJJBaseCharacter::HandleInput_Jump(const FInputActionValue& Value)
{
	HandleInput(InputActions.Jump, Value);
}
void AJJBaseCharacter::HandleInput_Jetpack(const FInputActionValue& Value)
{
	HandleInput(InputActions.Jetpack, Value);
}
void AJJBaseCharacter::HandleInput_Interact(const FInputActionValue& Value)
{
	HandleInput(InputActions.Interact, Value);
}
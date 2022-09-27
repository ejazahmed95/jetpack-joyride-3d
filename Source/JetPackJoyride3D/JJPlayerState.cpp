// Fill out your copyright notice in the Description page of Project Settings.


#include "JJPlayerState.h"

#include "JJBaseCharacter.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UJJPlayerState* UJJPlayerState::Init(AJJBaseCharacter* InPlayer)
{
	PlayerCharacter = InPlayer;
	return this;
}

void UJJPlayerState::BeginState()
{
}

UJJPlayerState* UJJPlayerState::Tick(float DeltaTime)
{
	return nullptr;
}

void UJJPlayerState::EndState()
{
}

UJJPlayerState* UJJPlayerState::HandleInput(UInputAction* InAction, const FInputActionValue& InValue)
{
	return nullptr;
}

void UJJPlayerState::MovePlayerForward(float InValue) const
{
	if((PlayerCharacter->Controller != nullptr) && (InValue != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = PlayerCharacter->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		PlayerCharacter->AddMovementInput(Direction, InValue);
	}
}

void UJJPlayerState::MovePlayerRight(float InValue) const
{
	if((PlayerCharacter->Controller != nullptr) && (InValue != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = PlayerCharacter->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		PlayerCharacter->AddMovementInput(Direction, InValue);
	}
}

/*
 * IDLE State
 */
UJJPlayerState* UIdleState::HandleInput(UInputAction* InAction, const FInputActionValue& InValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Input Handling in state = IDLE"));
	if(InAction == PlayerCharacter->InputActions.MoveForward) 
	{
		MovePlayerForward(InValue.GetMagnitude());
		return PlayerCharacter->AllStates.MovingState;
	}
	if(InAction == PlayerCharacter->InputActions.MoveRight) 
	{
		MovePlayerRight(InValue.GetMagnitude());
		return PlayerCharacter->AllStates.MovingState;
	}
	if(InAction == PlayerCharacter->InputActions.TurnRight) 
	{
		PlayerCharacter->AddControllerYawInput(InValue.GetMagnitude());
		return nullptr;
	}
	if(InAction == PlayerCharacter->InputActions.Jump)
	{
		UE_LOG(LogTemp, Warning, TEXT("Jump Action: %f"), InValue.GetMagnitude());
		PlayerCharacter->Jump();
		return PlayerCharacter->AllStates.JumpingState;
	}
	if(InAction == PlayerCharacter->InputActions.Interact)
	{
		PlayerCharacter->Interact();
		return PlayerCharacter->AllStates.InteractingState;
	}
	return nullptr;
}

void UMovingState::BeginState()
{
	Super::BeginState();
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerCharacter->FootStepAudio, FVector());
}

/*
 * MOVING STATE
 */
UJJPlayerState* UMovingState::HandleInput(UInputAction* InAction, const FInputActionValue& InValue)
{
	const float Speed = PlayerCharacter->GetCharacterMovement()->GetLastUpdateVelocity().Length();
	if(Speed == 0)
	{
		return PlayerCharacter->AllStates.IdleState;
	}
	UE_LOG(LogTemp, Warning, TEXT("Input Handling in state = MOVING Speed = %.2f"), Speed);
	if(InAction == PlayerCharacter->InputActions.MoveForward) 
	{
		MovePlayerForward(InValue.GetMagnitude());
		return PlayerCharacter->AllStates.MovingState;
	}
	if(InAction == PlayerCharacter->InputActions.MoveRight) 
	{
		MovePlayerRight(InValue.GetMagnitude());
		return PlayerCharacter->AllStates.MovingState;
	}
	if(InAction == PlayerCharacter->InputActions.TurnRight) 
	{
		PlayerCharacter->AddControllerYawInput(InValue.GetMagnitude());
		return nullptr;
	}
	if(InAction == PlayerCharacter->InputActions.Jump)
	{
		UE_LOG(LogTemp, Warning, TEXT("Jump Action: %f"), InValue.GetMagnitude());
		PlayerCharacter->Jump();
		return PlayerCharacter->AllStates.JumpingState;
	}
	return nullptr;
}


UJJPlayerState* UMovingState::Tick(float DeltaTime)
{
	if(PlayerCharacter->GetCharacterMovement()->GetLastUpdateVelocity().Length() == 0)
	{
		return PlayerCharacter->AllStates.IdleState;
	};
	return nullptr;
}


/*
 * IN_AIR STATE
 */
UJJPlayerState* UJumpingState::HandleInput(UInputAction* InAction, const FInputActionValue& InValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Input Handling in state = IN_AIR"));
	if(InAction == PlayerCharacter->InputActions.Jetpack)
	{
		if(InValue.GetMagnitude() != 0)
		{
			return PlayerCharacter->AllStates.JetpackActiveState;
		}
	}
	return Super::HandleInput(InAction, InValue);
}


UJJPlayerState* UJumpingState::Tick(float DeltaTime)
{
	if(PlayerCharacter->GetCharacterMovement()->IsFalling() == false)
	{
		if(PlayerCharacter->GetCharacterMovement()->GetLastUpdateVelocity().Length() == 0)
		{
			return PlayerCharacter->AllStates.IdleState;
		};
		return PlayerCharacter->AllStates.MovingState;
	}
	return nullptr;
}


/*
 * JETPACK STATE
 */
void UJetpackState::BeginState()
{

	Super::BeginState();
	PlayerCharacter->JetpackSound->Play();
	PlayerCharacter->IsJetpackActive = true;
}

UJJPlayerState* UJetpackState::HandleInput(UInputAction* InAction, const FInputActionValue& InValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Input Handling in state = JETPACK"));
	if(InAction == PlayerCharacter->InputActions.Jetpack)
	{
		if(InValue.GetMagnitude() == 0)
		{
			return PlayerCharacter->AllStates.JumpingState;
		}
	}
	return nullptr;
}

void UJetpackState::EndState()
{
	Super::EndState();
	PlayerCharacter->JetpackSound->Stop();
	PlayerCharacter->IsJetpackActive = false;
}

UJJPlayerState* UJetpackState::Tick(float DeltaTime)
{
	PlayerCharacter->LaunchCharacter(FVector(0.0f, 0.0f, 120.0f), false, true);
	return nullptr;
}

/*
 * INTERACTING STATE
 */
UJJPlayerState* UInteractingState::Tick(float DeltaTime)
{
	if(PlayerCharacter->IsInteracting == false)
	{
		return PlayerCharacter->AllStates.IdleState;
	}
	return nullptr;
}

/*
 * INTERACTING STATE
 */
UJJPlayerState* UInteractingState::HandleInput(UInputAction* InAction, const FInputActionValue& InValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Input Handling in state = INTERACTING"));
	if(InAction == PlayerCharacter->InputActions.TurnRight)
	{
		PlayerCharacter->AddControllerYawInput(InValue.GetMagnitude());
	}
	return nullptr;
}

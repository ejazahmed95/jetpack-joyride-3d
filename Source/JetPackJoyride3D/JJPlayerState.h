// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "UObject/Object.h"
#include "JJPlayerState.generated.h"

class AJJBaseCharacter;

/**
 * 
 */

UENUM()
enum EPlayerStateTransitionEvent {
	PlayerInputMoveForward,
	PlayerInputMoveRight,
	PlayerInputTurn,
	PlayerInputJump,
	PlayerInputJetpackOn,
	PlayerInputJetpackOff,
	PlayerInputInteractStart
};

UCLASS()
class JETPACKJOYRIDE3D_API UJJPlayerState : public UObject {
	GENERATED_BODY()

public:
	UJJPlayerState*         Init(AJJBaseCharacter* InPlayer);
	virtual void          BeginState();
	virtual UJJPlayerState*          Tick(float DeltaTime);
	virtual void          EndState();
	virtual UJJPlayerState* HandleInput(UInputAction* InAction, const FInputActionValue& InValue);

	UPROPERTY()
	AJJBaseCharacter* PlayerCharacter;

protected:
	void MovePlayerForward(float InValue) const;
	void MovePlayerRight(float InValue) const;
};

/*****************************************************
 * IDLE State
 ***************************************************/
UCLASS()
class JETPACKJOYRIDE3D_API UIdleState: public UJJPlayerState {
	GENERATED_BODY()
public:
	virtual UJJPlayerState* HandleInput(UInputAction* InAction, const FInputActionValue& InValue) override;
};

/*****************************************************
 * MOVING State
 ***************************************************/
UCLASS()
class UMovingState : public UJJPlayerState {
	GENERATED_BODY()
public:
	virtual void BeginState() override;
	virtual UJJPlayerState* Tick(float DeltaTime) override;
	virtual UJJPlayerState* HandleInput(UInputAction* InAction, const FInputActionValue& InValue) override;
};

/*****************************************************
 * JUMPING State
 ***************************************************/
UCLASS()
class UJumpingState : public UJJPlayerState {
	GENERATED_BODY()
public:
	virtual UJJPlayerState* Tick(float DeltaTime) override;
	virtual UJJPlayerState* HandleInput(UInputAction* InAction, const FInputActionValue& InValue) override;
};

/*****************************************************
 * JETPACK State
 ***************************************************/
UCLASS()
class UJetpackState : public UJJPlayerState {
	GENERATED_BODY()
public:
	virtual void BeginState() override;
	virtual UJJPlayerState* Tick(float DeltaTime) override;
	virtual UJJPlayerState* HandleInput(UInputAction* InAction, const FInputActionValue& InValue) override;
	virtual void EndState() override;
};

/*****************************************************
 * INTERACTING State
 ***************************************************/
UCLASS()
class UInteractingState : public UJJPlayerState {
	GENERATED_BODY()
public:
	virtual UJJPlayerState* Tick(float DeltaTime) override;
	virtual UJJPlayerState* HandleInput(UInputAction* InAction, const FInputActionValue& InValue) override;
};

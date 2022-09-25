// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "JJPlayerState.h"
#include "JJBaseCharacter.generated.h"

USTRUCT()
struct FInputActions {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveForward;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveRight;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* TurnRight;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* Jump;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* Jetpack;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* Interact;
};

USTRUCT()
struct FPlayerStates {
	GENERATED_BODY()

	UPROPERTY()
	UJJPlayerState* IdleState;
	UPROPERTY()
	UJJPlayerState* MovingState;
	UPROPERTY()
	UJJPlayerState* JumpingState;
	UPROPERTY()
	UJJPlayerState* JetpackActiveState;
	UPROPERTY()
	UJJPlayerState* InteractingState;
};

UCLASS()
class JETPACKJOYRIDE3D_API AJJBaseCharacter : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJJBaseCharacter();
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly)
	FInputActions InputActions;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* InteractionMontage;
	
	UPROPERTY()
	UJJPlayerState* CurrentState;

	UPROPERTY()
	FPlayerStates AllStates;

	FTimerHandle InteractionCompleteHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsInteracting;

protected:
	void SetupStates();
	void GoToState(UJJPlayerState* NewState);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void HandleInput(UInputAction* InputAction, const FInputActionValue& Value);

	UFUNCTION()
	void  Interact();

	UFUNCTION()
	void CompleteInteraction();

private:

	// New Bind Functions
	UFUNCTION()
	void HandleInput_MoveForward(const FInputActionValue& Value);

	UFUNCTION()
	void HandleInput_MoveRight(const FInputActionValue& Value);

	UFUNCTION()
	void HandleInput_Turn(const FInputActionValue& Value);

	UFUNCTION()
	void HandleInput_Jump(const FInputActionValue& Value);

	UFUNCTION()
	void HandleInput_Jetpack(const FInputActionValue& Value);

	UFUNCTION()
	void HandleInput_Interact(const FInputActionValue& Value);

};

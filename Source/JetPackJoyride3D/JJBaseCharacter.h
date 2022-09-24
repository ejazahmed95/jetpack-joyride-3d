// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void         TurnRight(const FInputActionValue& Value);
	void         MoveForward(const FInputActionValue& Value);
	void         MoveRight(const FInputActionValue& Value);


};

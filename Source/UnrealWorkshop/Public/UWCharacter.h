// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UWCharacter.generated.h"

class UUWInteractionComponent;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;

UCLASS()
class UNREALWORKSHOP_API AUWCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUWCharacter();

protected:

	UPROPERTY(EditAnywhere, Category = "UW|Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "UW|Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

	UPROPERTY(EditAnywhere, Category = "UW|Attack")
	float TimerDelay_PrimaryAttack = 0.2f;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UUWInteractionComponent* InteractComp;

	UPROPERTY(EditDefaultsOnly, Category = "UW|Input")
	UInputAction* Input_Move;

	UPROPERTY(EditDefaultsOnly, Category = "UW|Input")
	UInputAction* Input_Look;

	UPROPERTY(EditDefaultsOnly, Category = "UW|Input")
	UInputAction* Input_PrimaryAttack;
	
	UPROPERTY(EditDefaultsOnly, Category = "UW|Input")
	UInputAction* Input_Interact;

	UPROPERTY(EditDefaultsOnly, Category = "UW|Input")
	UInputMappingContext* DefaultInputMapping;

	void Move(const FInputActionValue& InputValue);

	void Look(const FInputActionValue& InputValue);

	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void PrimaryInteract();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

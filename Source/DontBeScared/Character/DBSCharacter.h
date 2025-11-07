// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DBSCharacter.generated.h"

class UAnimBlueprint;
class UInputMappingContext;
class UInputAction;
class UInputComponent;
class UCameraComponent;
class USkeletalMeshComponent;

UCLASS()
class DONTBESCARED_API ADBSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADBSCharacter();

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimBlueprint* FirstPersonDefaultAnim;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Soft pointers allow deferred loading, which helps reduce startup times and memory footprint by only loading assets when atually needed
	// Raw Pointers create hard dependencies and force assets to be in memory, potentially causing longer load times or higher memory usage upfront
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	TObjectPtr<UInputAction> LookMouseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void LookMouse(const FInputActionValue& Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void StartCrouch();

	UFUNCTION()
	void StopCrouch();

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USkeletalMeshComponent* FirstPersonMeshComponent;

	// These camera component settings allow Unreal Engine to render FirstPerson-tagged primitives differently to the player than they appear to the rest of the world, optimizing how they look and behave in first-person view.
	UPROPERTY(EditAnywhere, Category = "Camera")
	float FirstPersonFieldOfView = 70.0f;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float FirstPersonScale = 0.6f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector FirstPersonCameraOffset = FVector(2.8f, 5.9f, 0.0f);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

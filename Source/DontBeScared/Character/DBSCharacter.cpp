// Fill out your copyright notice in the Description page of Project Settings.


#include "DBSCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ADBSCharacter::ADBSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// Helps catch errors early if the component is missing or failed to initialize, preventing null pointer dereference crashes later.
	check(FirstPersonCameraComponent != nullptr);
	
	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FirstPersonMeshComponent != nullptr);
	
	FirstPersonMeshComponent->SetupAttachment(GetMesh());

	// FirstPerson-type primitives are rendered in a separate render pass, often with different camera parameters, so they don't cast a shadow. Player shadows perform best when cast from the third-person mesh
	FirstPersonMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	// This primitive type is for components that other players see. It automatically sets that component's OwnerNoSee property to false, making it invisible to the player. However, the mesh still casts a shadow. 
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;
	
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMeshComponent, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FirstPersonCameraOffset, FRotator(0.0f, 90.f, -90.0f));
	// This makes the camera inherit rotation from its parent Pawn so when the character turns, the camera follows
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent-> bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = FirstPersonFieldOfView;
	FirstPersonCameraComponent->FirstPersonScale = FirstPersonScale;

}

// Called when the game starts or when spawned
void ADBSCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Check if the global engine pointer is null before proceeding
	// Confirms engine systems are properly initialized before calling engine functions like AddOnScreenDebugMessage()
	// check only operates in Debug and Development builds (not shipping), so it has no performance impact on final builds
	check(GEngine != nullptr);

	FirstPersonMeshComponent->SetOnlyOwnerSee(true);
	FirstPersonMeshComponent->SetAnimInstanceClass(FirstPersonDefaultAnim->GeneratedClass);
	
}

// Called every frame
void ADBSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADBSCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();

	if (Controller)
	{
		
		const FVector Right = GetActorRightVector();
		AddMovementInput(Right, MovementValue.X);

		const FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, MovementValue.Y);
	}
}

void ADBSCharacter::LookMouse(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();

	if (Controller)
	{
		const float Yaw = MovementValue.X;
		AddControllerYawInput(Yaw);

		const float Pitch = MovementValue.Y;
		AddControllerPitchInput(Pitch);
	}
}

void ADBSCharacter::StartJump()
{
	// Controller is a pointer to the controller possessing this Actor, and it must be valid
	// for movement to work
	if (Controller)
	{
		// Built-in Jump function
		Jump();
	}
}

void ADBSCharacter::StopJump()
{
	if (Controller)
	{
		// Built-in StopJump function to reset values on Jump completed/canceled
		// Calls ResetJumpState()
		StopJumping();
	}
}

void ADBSCharacter::StartCrouch()
{
	if (Controller)
	{
		// Built in Crouch check, returns true if player CAN crouch
		if (CanCrouch())
		{
			Crouch();
		}
		
	}
}

void ADBSCharacter::StopCrouch()
{
	if (Controller)
	{
		// A simple toggle crouch logic. If player is crouched, then uncrouch when crouch key is pressed
		if (!CanCrouch())
		{
			UnCrouch();
		}
	}
}

// Called to bind functionality to input
void ADBSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Check if PlayerController cast is valid
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) 
	{
		// Check if Enhanced Input Local Player Subsystem cast is valid
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// From Subsystem, call function AddMappingContext with LoadSynchronous()
			// LoadSynchronous()  loads the referenced input mapping asset immediately and blocks the game thread until the asset is fully loaded.
			Subsystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);

			// Check if UEnhancedInputComponent cast is valid
			if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
			{
				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADBSCharacter::Move);
				EnhancedInputComponent->BindAction(LookMouseAction, ETriggerEvent::Triggered, this, &ADBSCharacter::LookMouse);
				EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADBSCharacter::StartJump);
				EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADBSCharacter::StopJump);
				EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ADBSCharacter::StartCrouch);
				EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ADBSCharacter::StopCrouch);
				
			}
			
		}
	}
}


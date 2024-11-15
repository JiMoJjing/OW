﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"

#include "OW/Interface/OWCharacterInputInterface.h"

DEFINE_LOG_CATEGORY(Log_Input_Controller);

AOWPlayerController::AOWPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	
	// InputMappingContext
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/OW/Input/IMC_OWDefault.IMC_OWDefault'"));
	if(DefaultMappingContextRef.Object)
	{
		DefaultMappingContext = DefaultMappingContextRef.Object;
	}

	// InputAction
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/OW/Input/InputAction/Movement/IA_OWJump.IA_OWJump'"));
	if(JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/OW/Input/InputAction/Movement/IA_OWMove.IA_OWMove'"));
	if(MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/OW/Input/InputAction/Movement/IA_OWLook.IA_OWLook'"));
	if(LookActionRef.Object)
	{
		LookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> PrimaryFireActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/OW/Input/InputAction/Ability/IA_OW_PrimaryFire.IA_OW_PrimaryFire'"));
	if(PrimaryFireActionRef.Object)
	{
		PrimaryFireAction = PrimaryFireActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SecondaryFireActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/OW/Input/InputAction/Ability/IA_OW_SecondaryFire.IA_OW_SecondaryFire'"));
	if(SecondaryFireActionRef.Object)
	{
		SecondaryFireAction = SecondaryFireActionRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> AbilityOneActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/OW/Input/InputAction/Ability/IA_OW_AbilityOne.IA_OW_AbilityOne'"));
	if(AbilityOneActionRef.Object)
	{
		AbilityOneAction = AbilityOneActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AbilityTwoActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/OW/Input/InputAction/Ability/IA_OW_AbilityTwo.IA_OW_AbilityTwo'"));
	if(AbilityTwoActionRef.Object)
	{
		AbilityTwoAction = AbilityTwoActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AbilityThreeActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/OW/Input/InputAction/Ability/IA_OW_AbilityThree.IA_OW_AbilityThree'"));
	if(AbilityThreeActionRef.Object)
	{
		AbilityThreeAction = AbilityThreeActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ReloadActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/OW/Input/InputAction/Ability/IA_OW_Reload.IA_OW_Reload'"));
	if(ReloadActionRef.Object)
	{
		ReloadAction = ReloadActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> QuickMeleeActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/OW/Input/InputAction/Ability/IA_OW_QuickMelee.IA_OW_QuickMelee'"));
	if(QuickMeleeActionRef.Object)
	{
		QuickMeleeAction = QuickMeleeActionRef.Object;
	}
}

void AOWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Input
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

void AOWPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UE_LOG(LogTemp, Warning, TEXT("OnPossess!"));

	// Character Reference
	if(IOWCharacterInputInterface* CharacterInputnInterface = Cast<IOWCharacterInputInterface>(InPawn))
	{
		CharacterInputInterface = TScriptInterface<IOWCharacterInputInterface>(InPawn);
	}

	if(IOWApplyDamageInterface* ApplyDamageInterface = Cast<IOWApplyDamageInterface>(InPawn))
	{
		CharacterApplyDamageInterface = TScriptInterface<IOWApplyDamageInterface>(InPawn);
	}
}

void AOWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if(EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AOWPlayerController::Jump);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOWPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOWPlayerController::Look);
		EnhancedInputComponent->BindAction(PrimaryFireAction, ETriggerEvent::Triggered, this, &AOWPlayerController::PrimaryFire);
		EnhancedInputComponent->BindAction(SecondaryFireAction, ETriggerEvent::Triggered, this, &AOWPlayerController::SecondaryFire);
		EnhancedInputComponent->BindAction(AbilityOneAction, ETriggerEvent::Triggered, this, &AOWPlayerController::AbilityOne);
		EnhancedInputComponent->BindAction(AbilityTwoAction, ETriggerEvent::Triggered, this, &AOWPlayerController::AbilityTwo);
		EnhancedInputComponent->BindAction(AbilityThreeAction, ETriggerEvent::Triggered, this, &AOWPlayerController::AbilityThree);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AOWPlayerController::Reload);
		EnhancedInputComponent->BindAction(QuickMeleeAction, ETriggerEvent::Triggered, this, &AOWPlayerController::QuickMelee);
	}
}

void AOWPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	const FRotator YawRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	APawn* ControlledPawn = GetPawn();
	if(ControlledPawn)
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputValue.X);
		ControlledPawn->AddMovementInput(RightDirection, InputValue.Y);
	}
}

void AOWPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	
	AddYawInput(InputValue.X);
	AddPitchInput(InputValue.Y);
}

void AOWPlayerController::Jump()
{
	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
	if(ControlledCharacter)
	{
		ControlledCharacter->Jump();
	}
}

void AOWPlayerController::PrimaryFire()
{
	UE_LOG(Log_Input_Controller, Warning, TEXT("PrimaryFire"));
	if(CharacterInputInterface)
	{
		CharacterInputInterface->PrimaryFire();
	}
}

void AOWPlayerController::SecondaryFire()
{
	UE_LOG(Log_Input_Controller, Warning, TEXT("SecondaryFire"));
	if(CharacterInputInterface)
	{
		CharacterInputInterface->SecondaryFire();
	}
}

void AOWPlayerController::AbilityOne()
{
	UE_LOG(Log_Input_Controller, Warning, TEXT("AbilityOne"));
	if(CharacterInputInterface)
	{
		CharacterInputInterface->AbilityOne();
	}
}

void AOWPlayerController::AbilityTwo()
{
	UE_LOG(Log_Input_Controller, Warning, TEXT("AbilityTwo"));
	if(CharacterInputInterface)
	{
		CharacterInputInterface->AbilityTwo();
	}
}

void AOWPlayerController::AbilityThree()
{
	UE_LOG(Log_Input_Controller, Warning, TEXT("AbilityThree"));
	if(CharacterInputInterface)
	{
		CharacterInputInterface->AbilityThree();
	}
}

void AOWPlayerController::Reload()
{
	UE_LOG(Log_Input_Controller, Warning, TEXT("Reload"));
	if(CharacterInputInterface)
	{
		CharacterInputInterface->Reload();
	}
}

void AOWPlayerController::QuickMelee()
{
	UE_LOG(Log_Input_Controller, Warning, TEXT("QuickMelee"));
	if(CharacterInputInterface)
	{
		CharacterInputInterface->QuickMelee();
	}
}

void AOWPlayerController::ApplyDamageSuccess(float Damage, bool bIsHeadShot)
{
	if(CharacterApplyDamageInterface)
	{
		CharacterApplyDamageInterface->ApplyDamageSuccess(Damage, bIsHeadShot);
	}
}

void AOWPlayerController::KillSuccess()
{
	if(CharacterApplyDamageInterface)
	{
		CharacterApplyDamageInterface->KillSuccess();
	}
}

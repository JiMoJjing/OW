// Fill out your copyright notice in the Description page of Project Settings.


#include "OWCharacterPlayable.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OW/Ability/AbilityComponent.h"
#include "OW/Ability/AbilityManagerComponent.h"

AOWCharacterPlayable::AOWCharacterPlayable()
{
	PrimaryActorTick.bCanEverTick = true;

	// SpringArm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.f, 100.f, 50.f);

	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	// AbilityComponent
	AbilityManagerComponent = CreateDefaultSubobject<UAbilityManagerComponent>(TEXT("AbilityManagerComponent"));

	// Collision
	BodyCollision->SetupAttachment(GetMesh(), TEXT("Body"));
	HeadCollision->SetupAttachment(GetMesh(), TEXT("Head"));
	
	UpperArmLCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("UpperArm_L"));
	LowerArmLCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LowerArm_L"));
	UpperArmLCollision->SetupAttachment(GetMesh(), TEXT("UpperArm_L"));
	LowerArmLCollision->SetupAttachment(GetMesh(), TEXT("LowerArm_L"));
	CollisionArray.AddUnique(UpperArmLCollision);
	CollisionArray.AddUnique(LowerArmLCollision);
	
	UpperArmRCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("UpperArm_R"));
	LowerArmRCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LowerArm_R"));
	UpperArmRCollision->SetupAttachment(GetMesh(),TEXT("UpperArm_R"));
	LowerArmRCollision->SetupAttachment(GetMesh(), TEXT("LowerArm_R"));
	CollisionArray.AddUnique(UpperArmRCollision);
	CollisionArray.AddUnique(LowerArmRCollision);

	ThighLCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Thigh_L"));
	CalfLCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Calf_L"));
	FootLCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Foot_L"));
	ThighLCollision->SetupAttachment(GetMesh(), TEXT("Thigh_L"));
	CalfLCollision->SetupAttachment(GetMesh(), TEXT("Calf_L"));
	FootLCollision->SetupAttachment(GetMesh(), TEXT("Foot_L"));
	CollisionArray.AddUnique(ThighLCollision);
	CollisionArray.AddUnique(CalfLCollision);
	CollisionArray.AddUnique(FootLCollision);
	
	ThighRCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Thigh_R"));
	CalfRCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Calf_R"));
	FootRCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Foot_R"));
	ThighRCollision->SetupAttachment(GetMesh(), TEXT("Thigh_R"));
	CalfRCollision->SetupAttachment(GetMesh(), TEXT("Calf_R"));
	FootRCollision->SetupAttachment(GetMesh(), TEXT("Foot_R"));
	CollisionArray.AddUnique(ThighRCollision);
	CollisionArray.AddUnique(CalfRCollision);
	CollisionArray.AddUnique(FootRCollision);
}

void AOWCharacterPlayable::BeginPlay()
{
	Super::BeginPlay();
}

void AOWCharacterPlayable::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if(APlayerController* PlayerController = Cast<APlayerController>(NewController))
	{
		OwnerController = PlayerController;
	}
}

void AOWCharacterPlayable::PrimaryFire()
{
	if(PrimaryFireComponent)
	{
		PrimaryFireComponent->UseAbility();
	}
}

void AOWCharacterPlayable::SecondaryFire()
{
	if(SecondaryFireComponent)
	{
		SecondaryFireComponent->UseAbility();
	}
}

void AOWCharacterPlayable::AbilityOne()
{
	if(AbilityOneComponent)
	{
		AbilityOneComponent->UseAbility();
	}
}

void AOWCharacterPlayable::AbilityTwo()
{
	if(AbilityTwoComponent)
	{
		AbilityTwoComponent->UseAbility();
	}
}

void AOWCharacterPlayable::AbilityThree()
{
	if(AbilityThreeComponent)
	{
		AbilityThreeComponent->UseAbility();
	}
}

void AOWCharacterPlayable::Reload()
{
	if(ReloadComponent)
	{
		ReloadComponent->UseAbility();
	}
}

void AOWCharacterPlayable::QuickMelee()
{
	if(QuickMeleeComponent)
	{
		QuickMeleeComponent->UseAbility();
	}
}

void AOWCharacterPlayable::SetIgnoreInput(bool bIgnore)
{
	if(Controller)
	{
		Controller->SetIgnoreMoveInput(bIgnore);
		Controller->SetIgnoreLookInput(bIgnore);
	}
}

void AOWCharacterPlayable::ApplyDamageSuccess(float Damage, bool bIsHeadShot)
{
	// ToDo : 궁극기 게이지 상승
}

void AOWCharacterPlayable::KillSuccess()
{
	
}

bool AOWCharacterPlayable::TraceUnderCrosshair(const float TraceDistance, FHitResult& OutHitResult,	FVector& OutHitLocation, const ECollisionChannel InCollisionChannel)
{
	if (nullptr == OwnerController) 
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s] PlayerController is nullptr"), *GetName(), TEXT("TraceUnderCrosshair"));
		return false;
	}
	
	// 뷰포트 크기 구하기
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// 스크린에서 크로스헤어 위치 구하기
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// 2D 화면 공간 좌표를 3D 공간 지점과 방향으로 변환
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(OwnerController, CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		const FVector TraceStartLocation = CrosshairWorldPosition;
		const FVector TraceEndLocation = TraceStartLocation + CrosshairWorldDirection * TraceDistance;

		if (UWorld* world = GetWorld())
		{
			world->LineTraceSingleByChannel(OutHitResult, TraceStartLocation, TraceEndLocation, InCollisionChannel);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s -> %s] GetWorld is nullptr"), *GetName(), TEXT("TraceUnderCrosshair"));
		}

		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
#if WITH_EDITOR
			UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation(), OutHitLocation, FLinearColor::Yellow, 2.f, 1.f);
#endif
			return true;
		}
		OutHitLocation = TraceEndLocation;
	}
	return false;
}

void AOWCharacterPlayable::GetDirectionToCrosshair(const FVector& StartLocation, FVector& OutDirection,	const ECollisionChannel InCollisionChannel)
{
	FVector HitLocation;
	FHitResult HitResult;

	bool bTrace = TraceUnderCrosshair(50000.f, HitResult, HitLocation, InCollisionChannel);

	if (bTrace)
	{
		FVector Direction = HitResult.Location - StartLocation;
		Direction.Normalize();
		OutDirection = Direction;
	}
	else
	{
		FVector Direction = HitLocation - StartLocation;
		Direction.Normalize();
		OutDirection = Direction;
	}
}

void AOWCharacterPlayable::TriggerAnimNotifyBegin()
{
	if(OnAnimNotifyBegin.IsBound())
	{
		OnAnimNotifyBegin.Broadcast();
	}
}

void AOWCharacterPlayable::TriggerAnimNotifyEnd()
{
	if(OnAnimNotifyEnd.IsBound())
	{
		OnAnimNotifyEnd.Broadcast();
	}
}

void AOWCharacterPlayable::TriggerAnimNotifyState(float DeltaTime)
{
	if(OnAnimNotifyState.IsBound())
	{
		OnAnimNotifyState.Broadcast(DeltaTime);
	}
}


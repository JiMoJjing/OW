// Fill out your copyright notice in the Description page of Project Settings.


#include "OWCharacterPlayable.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OW/ActorComponents/Ability/AbilityComponent.h"
#include "OW/Status/HPComponent.h"

AOWCharacterPlayable::AOWCharacterPlayable() : bQuickMeleeActive(false), CurrentAbilityType(EAbilityType::EAT_None), MaxUltimateGauge(2125.f), CurrentUltimateGauge(0.f), bUltimateActive(false), bIsJumpPressed(false)
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
	
// Collision CDO	
	UpperArmLCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("UpperArmL"));
	LowerArmLCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LowerArmL"));
	
	UpperArmRCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("UpperArmR"));
	LowerArmRCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LowerArmR"));
	
	ThighLCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ThighL"));
	CalfLCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CalfL"));
	FootLCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("FootL"));
	
	ThighRCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ThighR"));
	CalfRCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CalfR"));
	FootRCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("FootR"));

// SetupAttachment		
	UpperArmLCollision->SetupAttachment(GetMesh(), TEXT("UpperArm_L"));
	LowerArmLCollision->SetupAttachment(GetMesh(), TEXT("LowerArm_L"));
	
	UpperArmRCollision->SetupAttachment(GetMesh(), TEXT("UpperArm_R"));
	LowerArmRCollision->SetupAttachment(GetMesh(), TEXT("LowerArm_R"));
	
	ThighLCollision->SetupAttachment(GetMesh(), TEXT("Thigh_L"));
	CalfLCollision->SetupAttachment(GetMesh(), TEXT("Calf_L"));
	FootLCollision->SetupAttachment(GetMesh(), TEXT("Foot_L"));
	
	ThighRCollision->SetupAttachment(GetMesh(), TEXT("Thigh_R"));
	CalfRCollision->SetupAttachment(GetMesh(), TEXT("Calf_R"));
	FootRCollision->SetupAttachment(GetMesh(), TEXT("Foot_R"));
	
	UpperArmLCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	UpperArmLCollision->SetRelativeLocation(FVector(8.f, 0.f, 0.f));
	UpperArmLCollision->SetCapsuleRadius(8.f);
	UpperArmLCollision->SetCapsuleHalfHeight(18.f);
	
	LowerArmLCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	LowerArmLCollision->SetCapsuleRadius(6.f);
	LowerArmLCollision->SetCapsuleHalfHeight(16.f);
	
	ThighLCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	ThighLCollision->SetCapsuleRadius(10.f);
	ThighLCollision->SetCapsuleHalfHeight(26.f);
	
	CalfLCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	CalfLCollision->SetCapsuleRadius(8.f);
	CalfLCollision->SetCapsuleHalfHeight(26.f);
	
	FootLCollision->SetRelativeLocation(FVector(8.f, 0.f, 8.f));
	FootLCollision->SetCapsuleRadius(6.f);
	FootLCollision->SetCapsuleHalfHeight(16.f);
	
	UpperArmRCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	UpperArmRCollision->SetRelativeLocation(FVector(-8.f, 0.f, 0.f));
	UpperArmRCollision->SetCapsuleRadius(8.f);
	UpperArmRCollision->SetCapsuleHalfHeight(18.f);
	
	LowerArmRCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	LowerArmRCollision->SetCapsuleRadius(6.f);
	LowerArmRCollision->SetCapsuleHalfHeight(16.f);
	
	ThighRCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	ThighRCollision->SetCapsuleRadius(10.f);
	ThighRCollision->SetCapsuleHalfHeight(26.f);
	
	CalfRCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	CalfRCollision->SetCapsuleRadius(8.f);
	CalfRCollision->SetCapsuleHalfHeight(26.f);
	
	FootRCollision->SetRelativeLocation(FVector(-8.f, 0.f, -8.f));
	FootRCollision->SetCapsuleRadius(6.f);
	FootRCollision->SetCapsuleHalfHeight(16.f);
}

void AOWCharacterPlayable::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// Add to CollisionArray
	CollisionArray.AddUnique(UpperArmLCollision);
	CollisionArray.AddUnique(LowerArmLCollision);
	
	CollisionArray.AddUnique(UpperArmRCollision);
	CollisionArray.AddUnique(LowerArmRCollision);
	
	CollisionArray.AddUnique(ThighLCollision);
	CollisionArray.AddUnique(CalfLCollision);
	CollisionArray.AddUnique(FootLCollision);
	
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
	AutoAddUltimateGaugeTimerStart();
}

void AOWCharacterPlayable::SetIgnoreInput(bool bIgnore)
{
	if(Controller)
	{
		Controller->SetIgnoreMoveInput(bIgnore);
		Controller->SetIgnoreLookInput(bIgnore);
	}
}

void AOWCharacterPlayable::SetQuickMeleeActive(bool bActive)
{
	bQuickMeleeActive = bActive;
}

void AOWCharacterPlayable::AbilityStart(EAbilityType InAbilityType)
{
	CurrentAbilityType = InAbilityType;
	if(OnAbilityStart.IsBound())
	{
		OnAbilityStart.Broadcast(CurrentAbilityType);
	}
}

void AOWCharacterPlayable::AbilityEnd(EAbilityType InAbilityType)
{
	if(CurrentAbilityType == InAbilityType)
	{
		CurrentAbilityType = EAbilityType::EAT_None;
	}
	if(OnAbilityEnd.IsBound())
	{
		OnAbilityEnd.Broadcast(CurrentAbilityType);
	}
}

void AOWCharacterPlayable::AddAbilityStateChangedDelegate(const EAbilityType InAbilityType, const FOnAbilityStateChangedDelegateWrapper& InDelegateWrapper)
{
	AbilityStateChangedDelegateWrappers.Emplace(InAbilityType, InDelegateWrapper);
}

void AOWCharacterPlayable::AddAbilityCooldownTimeChangedDelegate(const EAbilityType InAbilityType, const FOnAbilityCooldownTimeChangedDelegateWrapper& InDelegateWrapper)
{
	AbilityCooldownTimeChangedDelegateWrappers.Emplace(InAbilityType, InDelegateWrapper);
}

void AOWCharacterPlayable::ApplyDamageSuccess(float Damage, bool bIsHeadShot)
{
	AddUltimateGauge(Damage);
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
			//UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation(), OutHitLocation, FLinearColor::Yellow, 2.f, 1.f);
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

void AOWCharacterPlayable::TriggerAnimNotify()
{
	if(OnAnimNotify.IsBound())
	{
		OnAnimNotify.Broadcast();
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
	UE_LOG(LogTemp, All, TEXT("End Called"));
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

void AOWCharacterPlayable::AddUltimateGauge(float InAmount)
{
	if(bUltimateActive == false)
	{
		CurrentUltimateGauge = FMath::Clamp(CurrentUltimateGauge + InAmount, 0.f, MaxUltimateGauge);
	}

	if(OnUltimateGaugeChanged.IsBound())
	{
		OnUltimateGaugeChanged.Broadcast(MaxUltimateGauge, CurrentUltimateGauge);
	}
}

void AOWCharacterPlayable::AutoAddUltimateGaugeTimerStart()
{
	GetWorldTimerManager().SetTimer(AutoAddUltimateGaugeTimerHandle, this, &AOWCharacterPlayable::AutoAddUltimateGauge, 1.f, true, 1.f);
}

void AOWCharacterPlayable::AutoAddUltimateGauge()
{
	AddUltimateGauge(5.f);
}

void AOWCharacterPlayable::SetUltimateActive(bool bActive)
{
	bUltimateActive = bActive;
}

void AOWCharacterPlayable::UltimateUsed()
{
	AddUltimateGauge(-MaxUltimateGauge);
}

void AOWCharacterPlayable::InitWidget()
{
	HPComponent->InitializeWidget();

	if(OnUltimateGaugeChanged.IsBound())
	{
		OnUltimateGaugeChanged.Broadcast(MaxUltimateGauge, CurrentUltimateGauge);
	}
}


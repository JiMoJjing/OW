// Fill out your copyright notice in the Description page of Project Settings.


#include "GenjiSwiftStrikeComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OW/Character/OWCharacterPlayable.h"
#include "OW/Character/Genji/OWGenji.h"

#include "OW/Collision/OWCollisionProfile.h"


UGenjiSwiftStrikeComponent::UGenjiSwiftStrikeComponent() : SwiftStrikeDistance(1884.f), SwiftStrikeSpeed(5000.f), SwiftStrikeStartLocation(FVector::ZeroVector),
SwiftStrikeEndLocation(FVector::ZeroVector), SwiftStrikeHitNormalProjection(FVector::ZeroVector), HitNormalProjectionInterpSpeed(10.f), bCheckDoubleJump(false),
bSwiftStrikeCooldownReset(false), CapsuleSize2D(0.f, 0.f), SwiftStrikeDamage(50.f), DragonbladeStartSectionName(TEXT("Section_03")), DragonbladeEndSectionName(TEXT("Section_04"))
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SwiftStrikeMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_Swiftstrike.AM_Genji_Swiftstrike'"));
	if(SwiftStrikeMontageRef.Object)
	{
		AbilityMontage = SwiftStrikeMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SwiftStrikeHitEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/OW/Genji/Niagara/NS_Genji_SwiftstrikeHit.NS_Genji_SwiftstrikeHit'"));
	if(SwiftStrikeHitEffectRef.Object)
	{
		SwiftStrikeHitEffect = SwiftStrikeHitEffectRef.Object;
	}
	

	SwiftStrikeCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SwiftStrikeCollider"));
	SwiftStrikeCollider->OnComponentBeginOverlap.AddDynamic(this, &UGenjiSwiftStrikeComponent::OnSwiftStrikeColliderBeginOverlap);
	SwiftStrikeCollider->InitSphereRadius(90.f);
	SwiftStrikeCollider->SetCollisionProfileName(OWTEAM1OVERLAP);
	SwiftStrikeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CooldownTime = 8.f;
	AbilityType = EAbilityType::EAT_AbilityOne;
}

void UGenjiSwiftStrikeComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if(CharacterPlayable)
	{
		CapsuleSize2D = FVector2D(CharacterPlayable->GetCapsuleComponent()->GetScaledCapsuleRadius(), CharacterPlayable->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		CharacterPlayable->GetCharacterMovement()->MaxFlySpeed = SwiftStrikeSpeed;
	}
	if(SwiftStrikeCollider)
	{
		SwiftStrikeCollider->AttachToComponent(CharacterPlayable->GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
	if(AOWGenji* Genji = Cast<AOWGenji>(GetOwner()))
	{
		GenjiRef = Genji;
	}
}

void UGenjiSwiftStrikeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGenjiSwiftStrikeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGenjiSwiftStrikeComponent::AbilityStart()
{
	Super::AbilityStart();

	SwiftStrikeStartSetting();

	if(GenjiRef->IsUltimateActive())
	{
		PlayAbilityMontage_JumpToSection(AbilityMontage, DragonbladeStartSectionName);
	}
	else
	{
		PlayAbilityMontage(AbilityMontage);
	}
}

void UGenjiSwiftStrikeComponent::AbilityEnd()
{
	Super::AbilityEnd();
	SwiftStrikeEndSetting();
	CooldownStart();
	
	if(bSwiftStrikeCooldownReset)
	{
		UE_LOG(LogTemp, Warning, TEXT("bSwiftSTrikeCooldownReset = True"));
		bSwiftStrikeCooldownReset = false;
		CooldownEnd();
	}
}


void UGenjiSwiftStrikeComponent::SwiftStrikeStartSetting()
{	
	SetSwiftStrikeStartLocation();
	SetSwiftStrikeEndLocation();

	bCheckDoubleJump = CharacterPlayable->JumpCurrentCount == CharacterPlayable->JumpMaxCount;
	bSwiftStrikeCooldownReset = false;

	CharacterPlayable->SetIgnoreInput(true);
	CharacterPlayable->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	CharacterPlayable->GetCharacterMovement()->MaxAcceleration = 1000000.f;
	CharacterPlayable->OnAnimNotifyState.AddUObject(this, &UGenjiSwiftStrikeComponent::SwiftStrikeUptade);
	CharacterPlayable->OnAnimNotifyEnd.AddUObject(this, &UGenjiSwiftStrikeComponent::AbilityEnd);
	CharacterPlayable->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECR_Ignore);
	CharacterPlayable->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UGenjiSwiftStrikeComponent::OnCapsuleComponentHit);

	SwiftStrikeCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OverlappedActors.Empty();
	
	if(AbilityMontage)
	{
		AbilityMontage->bEnableAutoBlendOut = false;
	}	
}

void UGenjiSwiftStrikeComponent::SwiftStrikeEndSetting()
{
	CharacterPlayable->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	CharacterPlayable->GetCharacterMovement()->MaxAcceleration = 2048.f;
	CharacterPlayable->GetCharacterMovement()->StopMovementImmediately();
	CharacterPlayable->SetIgnoreInput(false);
	CharacterPlayable->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECR_Block);
	CharacterPlayable->GetCapsuleComponent()->OnComponentHit.RemoveAll(this);
	CharacterPlayable->OnAnimNotifyState.RemoveAll(this);
	CharacterPlayable->OnAnimNotifyEnd.RemoveAll(this);
	
	SwiftStrikeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if(bCheckDoubleJump)
	{
		CharacterPlayable->JumpCurrentCount = 2;
	}

	bCheckDoubleJump = false;

	SwiftStrikeStartLocation = FVector::ZeroVector;
	SwiftStrikeEndLocation = FVector::ZeroVector;
	SwiftStrikeHitNormalProjection = FVector::ZeroVector;

	AbilityMontage->bEnableAutoBlendOut = true;

	if(GenjiRef->IsUltimateActive())
	{
		PlayAbilityMontage_JumpToSection(AbilityMontage, DragonbladeEndSectionName);
	}
	else
	{
		PlayAbilityMontage_JumpToSection(AbilityMontage, TEXT("Section_02"));
	}
	
}

void UGenjiSwiftStrikeComponent::SetSwiftStrikeStartLocation()
{
	SwiftStrikeStartLocation = CharacterPlayable->GetActorLocation();
}

void UGenjiSwiftStrikeComponent::SetSwiftStrikeEndLocation()
{
	FHitResult HitResult;
	FVector TraceEndLocation;

	bool bLineTrace = CharacterPlayable->TraceUnderCrosshair(SwiftStrikeDistance, HitResult, TraceEndLocation, ECC_Camera);
	if(bLineTrace)
	{
		const FVector HitNormal = HitResult.Normal;
		const FVector AdjustedVector(HitNormal.X * CapsuleSize2D.X, HitNormal.Y * CapsuleSize2D.X, HitNormal.Z * CapsuleSize2D.Y);
		SwiftStrikeEndLocation = HitResult.Location + AdjustedVector;
	}
	else
	{
		SwiftStrikeEndLocation = TraceEndLocation;
	}

#if WITH_EDITOR
	// Draw Debug
	DrawDebugSphere(GetWorld(), SwiftStrikeEndLocation, 10.f, 20, FColor::Green, false, 5.f, 0U, 1.f);
	DrawDebugLine(GetWorld(), CharacterPlayable->GetActorLocation(), SwiftStrikeEndLocation, FColor::Green, false, 5.f, 0U, 1.f);
#endif
}

void UGenjiSwiftStrikeComponent::SwiftStrikeUptade(float DeltaTime)
{
	FVector ActorLocation = CharacterPlayable->GetActorLocation();
	FVector Direction = SwiftStrikeEndLocation - ActorLocation;
	Direction.Normalize();

	Direction += SwiftStrikeHitNormalProjection;
	Direction.Normalize();
	
	CharacterPlayable->AddMovementInput(Direction, 1.f, true);

	SwiftStrikeHitNormalProjection = FMath::VInterpTo(SwiftStrikeHitNormalProjection, FVector::ZeroVector, DeltaTime, HitNormalProjectionInterpSpeed);

	FVector EndLocation = SwiftStrikeEndLocation;
	EndLocation.Z = 0.f;
	ActorLocation.Z = 0.f;
	if(ActorLocation.Equals(EndLocation, 20.f))
	{
		AbilityEnd();
	}
}

void UGenjiSwiftStrikeComponent::OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if(Hit.bBlockingHit && AbilityState == EAbilityState::EAS_Active)
	{
		const FVector HitNormal = Hit.Normal;
		const FVector ActorForwardVector = CharacterPlayable->GetActorForwardVector();
		// 평면 투영 벡터 공식을 사용한 방법 Vp = V - (V dot N) * N
		SwiftStrikeHitNormalProjection = ActorForwardVector - FVector::DotProduct(ActorForwardVector, HitNormal) * HitNormal;
	}
}

void UGenjiSwiftStrikeComponent::OnSwiftStrikeColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AOWCharacterBase* CharacterBase = Cast<AOWCharacterBase>(OtherComp->GetOwner()))
	{
		if(OverlappedActors.Find(CharacterBase) != INDEX_NONE)
		{
			return;
		}
		OverlappedActors.AddUnique(CharacterBase);
		UGameplayStatics::ApplyDamage(CharacterBase, SwiftStrikeDamage, CharacterPlayable->GetController(), GetOwner(), UDamageType::StaticClass());

		const FVector EffectLocation = OtherComp->GetComponentLocation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(OtherComp, SwiftStrikeHitEffect, EffectLocation, FRotator::ZeroRotator);
	}
}

void UGenjiSwiftStrikeComponent::SwiftStrikeCooldownReset()
{
	if(AbilityState == EAbilityState::EAS_Active)
	{
		bSwiftStrikeCooldownReset = true;
	}
	else
	{
		CooldownEnd();
	}
}
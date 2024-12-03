// Fill out your copyright notice in the Description page of Project Settings.


#include "OWStandardBot.h"

#include "Components/CapsuleComponent.h"
#include "OW/ActorComponents/BasicAttack/ProjectileWeaponComponent.h"
#include "OW/Collision/OWCollisionProfile.h"
#include "OW/Projectile/OWProjectileBase.h"
#include "OW/Widget/WidgetComponent/HPBarWidgetComponent.h"


AOWStandardBot::AOWStandardBot() : bMuzzleChange(true)
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/OW_Assets/StandardBot/StandardBot.StandardBot'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/OW/StandardBot/AnimInstance/ABP_StandardBot.ABP_StandardBot_C"));
	if(AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReviveAnimMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/StandardBot/AnimMontage/AM_StandardBot_Revive.AM_StandardBot_Revive'"));
	if(ReviveAnimMontageRef.Object)
	{
		ReviveAnimMontage = ReviveAnimMontageRef.Object;
	}
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.f);
	GetCapsuleComponent()->SetCapsuleRadius(30.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	HPBarWidgetComponent = CreateDefaultSubobject<UHPBarWidgetComponent>(TEXT("HPBarWidgetComponent"));
	HPBarWidgetComponent->SetupAttachment(RootComponent);
	HPBarWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	HPBarWidgetComponent->SetDrawSize(FVector2D(100.f, 100.f));

	ArmLCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Arm_L"));
	ArmRCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Arm_R"));
	LegCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Leg"));
	
	ArmLCollision->SetupAttachment(GetMesh(), TEXT("Arm_L"));
	ArmRCollision->SetupAttachment(GetMesh(), TEXT("Arm_R"));
	LegCollision->SetupAttachment(GetMesh(), TEXT("Leg"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> StandardBotFireMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/StandardBot/AnimMontage/AM_StandardBot_Fire.AM_StandardBot_Fire'"));
	if(StandardBotFireMontageRef.Object)
	{
		StandardBotFireMontage = StandardBotFireMontageRef.Object;
	}

	ProjectileWeaponComponent = CreateDefaultSubobject<UProjectileWeaponComponent>(TEXT("ProjectileWeaponComponent"));

	LeftMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("LeftMuzzle"));
	RightMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("RightMuzzle"));
	LeftMuzzle->SetupAttachment(GetMesh(), TEXT("bone_08B2"));
	RightMuzzle->SetupAttachment(GetMesh(), TEXT("bone_08B3"));
}

void AOWStandardBot::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CollisionArray.AddUnique(ArmLCollision);
	CollisionArray.AddUnique(ArmRCollision);
	CollisionArray.AddUnique(LegCollision);
		
	GetCapsuleComponent()->SetCollisionProfileName(OWTEAM2CAPSULE);
	SetMeshCollisionProfileName(OWTEAM2MESH);
}

void AOWStandardBot::BeginPlay()
{
	Super::BeginPlay();

	if(HPBarWidgetComponent)
	{
		HPBarWidgetComponent->SetNameText(FText::FromName(TEXT("Standard Bot")));
		SetWidgetComponentVisibility(false);
	}
}

void AOWStandardBot::CharacterDeath()
{
	Super::CharacterDeath();

	SetWidgetComponentVisibility(false);

	FTimerHandle ReviveTimerHandle;
	GetWorldTimerManager().SetTimer(ReviveTimerHandle, this, &AOWStandardBot::CharacterRevive, 5.f, false);
}

void AOWStandardBot::CharacterRevive()
{
	Super::CharacterRevive();
	GetMesh()->GetAnimInstance()->Montage_Play(ReviveAnimMontage);
}

float AOWStandardBot::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,	AActor* DamageCauser)
{
	SetWidgetComponentVisibility(true);
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AOWStandardBot::SetWidgetComponentVisibility(bool bNewVisibility)
{
	HPBarWidgetComponent->SetVisibility(bNewVisibility);
}

void AOWStandardBot::PlayFireMontage()
{
	GetMesh()->GetAnimInstance()->Montage_Play(StandardBotFireMontage);
}

void AOWStandardBot::StandardBotFire()
{
	FVector ShotDirection = GetActorForwardVector();
	AOWProjectileBase* Projectile = ProjectileWeaponComponent->GetProjectileFromPool();

	if(Projectile)
	{
		if(bMuzzleChange & true)
		{
			Projectile->ActivateProjectile(LeftMuzzle->GetComponentLocation(), ShotDirection);
		}
		else
		{
			Projectile->ActivateProjectile(RightMuzzle->GetComponentLocation(), ShotDirection);
		}
		bMuzzleChange = !bMuzzleChange;
	}
}

void AOWStandardBot::TriggerAnimNotify()
{
	StandardBotFire();
}

void AOWStandardBot::TriggerAnimNotifyBegin()
{
}

void AOWStandardBot::TriggerAnimNotifyEnd()
{
}

void AOWStandardBot::TriggerAnimNotifyState(float Delta)
{
}

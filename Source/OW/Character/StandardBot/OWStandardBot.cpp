// Fill out your copyright notice in the Description page of Project Settings.


#include "OWStandardBot.h"

#include "Components/CapsuleComponent.h"
#include "OW/ActorComponents/BasicAttack/ProjectileWeaponComponent.h"
#include "OW/AI/OWAIController.h"
#include "OW/Collision/OWCollisionProfile.h"
#include "OW/Projectile/OWProjectileBase.h"
#include "OW/Widget/WidgetComponent/HPBarWidgetComponent.h"


AOWStandardBot::AOWStandardBot() : PoolSize(40), PoolIndex(0), bMuzzleChange(true), AIDetectRange(1000.f), AIAttackRange(1500.f), AITurnSpeed(2.f), AIPatrolPos(FVector::ZeroVector)
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

	static ConstructorHelpers::FClassFinder<AOWProjectileBase> ProjectileClassRef(TEXT("/Game/OW/StandardBot/Blueprint/BP_StandardBot_Projectile.BP_StandardBot_Projectile_C"));
	if(ProjectileClassRef.Class)
	{
		ProjectileClass = ProjectileClassRef.Class;
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

	LeftMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("LeftMuzzle"));
	RightMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("RightMuzzle"));
	LeftMuzzle->SetupAttachment(GetMesh(), TEXT("bone_08B2"));
	RightMuzzle->SetupAttachment(GetMesh(), TEXT("bone_08B3"));

	AIControllerClass = AOWAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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
		SpawnProjectile();
	}

	if(AOWAIController* AIController = Cast<AOWAIController>(GetController()))
	{
		OWAIController = AIController;
	}
}

void AOWStandardBot::CharacterDeath()
{
	Super::CharacterDeath();

	SetWidgetComponentVisibility(false);
	OWAIController->StopAI();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AOWStandardBot::CharacterRevive, 4.f, false);
}

void AOWStandardBot::CharacterRevive()
{
	Super::CharacterRevive();
	GetMesh()->GetAnimInstance()->Montage_Play(ReviveAnimMontage);
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AOWStandardBot::CharacterReviveRunAI, 3.f, false);
}

void AOWStandardBot::CharacterReviveRunAI()
{
	if(OWAIController)
	{
		OWAIController->RunAI();
	}
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
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance)
	{
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AOWStandardBot::AttackFinished);
		AnimInstance->Montage_Play(StandardBotFireMontage);
		AnimInstance->Montage_SetEndDelegate(EndDelegate);
	}
}

void AOWStandardBot::StandardBotFire()
{
	AOWProjectileBase* Projectile = GetProjectileFromPool();
	FVector DirectionVec = GetActorForwardVector();
	if(Projectile)
	{
		if(bMuzzleChange & true)
		{
			Projectile->ActivateProjectile(LeftMuzzle->GetComponentLocation(), DirectionVec);
		}
		else
		{
			Projectile->ActivateProjectile(RightMuzzle->GetComponentLocation(), DirectionVec);
		}
		bMuzzleChange = !bMuzzleChange;
	}
}

void AOWStandardBot::AttackFinished(UAnimMontage* Montage, bool bInterrupted)
{
	OnAttachFinished.ExecuteIfBound();
}

void AOWStandardBot::SpawnProjectile()
{
	if(ProjectileClass)
	{
		UWorld* World = GetOwner()->GetWorld();
		FActorSpawnParameters ActorSpawnParameter;
		ActorSpawnParameter.Instigator = this;
		ActorSpawnParameter.Owner = this;
		
		for(uint8 index = 0; index < PoolSize; index++)
		{
			AOWProjectileBase* Projectile = World->SpawnActor<AOWProjectileBase>(ProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator, ActorSpawnParameter);

			if(Projectile)
			{
				ProjectilePool.Add(Projectile);
			}
		}
	}
}

AOWProjectileBase* AOWStandardBot::GetProjectileFromPool()
{
	if(ProjectilePool.IsEmpty())
	{
		return nullptr;
	}
		
	PoolIndex %= PoolSize;

	AOWProjectileBase* ProjectileBase = ProjectilePool[PoolIndex++];

	return ProjectileBase;
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

float AOWStandardBot::GetAIDetectRange()
{
	return AIDetectRange;
}

float AOWStandardBot::GetAIAttackRange()
{
	return AIAttackRange;
}

float AOWStandardBot::GetAITrunSpeed()
{
	return AITurnSpeed;
}

FVector AOWStandardBot::GetAIPatrolPos()
{
	return AIPatrolPos;
}

ECollisionChannel AOWStandardBot::GetAITraceChannel()
{
	return OWTEAM2TRACE;
}

void AOWStandardBot::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttachFinished = InOnAttackFinished;
}

void AOWStandardBot::AIAttack()
{
	PlayFireMontage();
}

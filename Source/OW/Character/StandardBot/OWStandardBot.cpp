// Fill out your copyright notice in the Description page of Project Settings.


#include "OWStandardBot.h"

#include "Components/CapsuleComponent.h"
#include "OW/Collision/OWCollisionProfile.h"
#include "OW/Widget/WidgetComponent/HPBarWidgetComponent.h"


AOWStandardBot::AOWStandardBot()
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

	ArmLCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ArmL"));
	ArmRCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ArmR"));
	LegCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Leg"));

	CollisionArray.AddUnique(ArmLCollision);
	CollisionArray.AddUnique(ArmRCollision);
	CollisionArray.AddUnique(LegCollision);
	
	BodyCollision->SetupAttachment(GetMesh(), TEXT("Body"));
	HeadCollision->SetupAttachment(GetMesh(), TEXT("Head"));
	ArmLCollision->SetupAttachment(GetMesh(), TEXT("Arm_L"));
	ArmRCollision->SetupAttachment(GetMesh(), TEXT("Arm_R"));
	LegCollision->SetupAttachment(GetMesh(), TEXT("Leg"));
}

void AOWStandardBot::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetCapsuleComponent()->SetCollisionProfileName(OWTEAM2CAPSULE);
	SetCollisionProfileName(OWTEAM2MESH);
}

void AOWStandardBot::BeginPlay()
{
	Super::BeginPlay();

	if(HPBarWidgetComponent)
	{
		HPBarWidgetComponent->SetNameText(FText::FromName(TEXT("Standard Bot")));
	}
}

void AOWStandardBot::CharacterDeath()
{
	Super::CharacterDeath();

	FTimerHandle ReviveTimerHandle;
	GetWorldTimerManager().SetTimer(ReviveTimerHandle, this, &AOWStandardBot::CharacterRevive, 5.f, false);
}

void AOWStandardBot::CharacterRevive()
{
	Super::CharacterRevive();
	GetMesh()->GetAnimInstance()->Montage_Play(ReviveAnimMontage);
}

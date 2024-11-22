


#include "OWGenji.h"

#include "Components/CapsuleComponent.h"
#include "OW/Ability/Genji/GenjiQuickMeleeComponent.h"
#include "OW/Ability/Genji/GenjiShurikenComponent.h"
#include "OW/Ability/Genji/GenjiSwiftStrikeComponent.h"
#include "OW/Collision/OWCollisionProfile.h"

AOWGenji::AOWGenji()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/OW_Genji_Assets/OW_Genji.OW_Genji'"));
	if(SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/OW/Genji/AnimInstance/ABP_Genji.ABP_Genji_C"));
	if(AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	AbilityOneComponent = CreateDefaultSubobject<UGenjiSwiftStrikeComponent>(TEXT("SwiftStrikeComponent"));
	QuickMeleeComponent = CreateDefaultSubobject<UGenjiQuickMeleeComponent>(TEXT("QuickMeleeComponent"));
	PrimaryFireComponent = CreateDefaultSubobject<UGenjiShurikenComponent>(TEXT("ShurikenComponent"));

	
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.f);
	
	GetCapsuleComponent()->SetCapsuleRadius(30.f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	
	JumpMaxCount = 2;
	
	// AttachToComponent
	BodyCollision->SetupAttachment(GetMesh(), TEXT("Body"));
	HeadCollision->SetupAttachment(GetMesh(), TEXT("Head"));

	UpperArmLCollision->SetupAttachment(GetMesh(), TEXT("UpperArm_L"));
	LowerArmLCollision->SetupAttachment(GetMesh(), TEXT("LowerArm_L"));

	UpperArmRCollision->SetupAttachment(GetMesh(),TEXT("UpperArm_R"));
	LowerArmRCollision->SetupAttachment(GetMesh(), TEXT("LowerArm_R"));

	ThighLCollision->SetupAttachment(GetMesh(), TEXT("Thigh_L"));
	CalfLCollision->SetupAttachment(GetMesh(), TEXT("Calf_L"));
	FootLCollision->SetupAttachment(GetMesh(), TEXT("Foot_L"));

	ThighRCollision->SetupAttachment(GetMesh(), TEXT("Thigh_R"));
	CalfRCollision->SetupAttachment(GetMesh(), TEXT("Calf_R"));
	FootRCollision->SetupAttachment(GetMesh(), TEXT("Foot_R"));

	// SetRelativeLocation and Rotation
	for(auto& Collision : CollisionArray)
	{
		Collision->SetRelativeScale3D(FVector(0.01f, 0.01f, 0.01f));
	}
	
	BodyCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	
	HeadCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	HeadCollision->SetRelativeLocation(FVector(0.f, -0.15f, 0.f));

	UpperArmLCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	UpperArmLCollision->SetRelativeLocation(FVector(0.08f, 0.f, 0.f));

	LowerArmLCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	ThighLCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	CalfLCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	FootLCollision->SetRelativeLocation(FVector(0.08f, 0.f, 0.08f));
	
	UpperArmRCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	UpperArmRCollision->SetRelativeLocation(FVector(-0.08f, 0.f, 0.f));
	
	LowerArmRCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	
	ThighRCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	CalfRCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	FootRCollision->SetRelativeLocation(FVector(-0.08f, 0.f, -0.08f));
	
	// Collision Profile Setting 임시
	GetCapsuleComponent()->SetCollisionProfileName(OWTEAM1CAPSULE);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	SetCollisionProfileName(OWTEAM1MESH);
}

void AOWGenji::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AbilityComponents.Add(AbilityOneComponent->GetAbilityType(), AbilityOneComponent);
	AbilityComponents.Add(QuickMeleeComponent->GetAbilityType(), QuickMeleeComponent);
	AbilityComponents.Add(PrimaryFireComponent->GetAbilityType(), PrimaryFireComponent);

	GenjiShurikenComponent = Cast<UGenjiShurikenComponent>(PrimaryFireComponent);
}

void AOWGenji::SecondaryFire()
{
	if(GenjiShurikenComponent)
	{
		GenjiShurikenComponent->UseSecondAbility();
	}
}

void AOWGenji::BeginPlay()
{
	Super::BeginPlay();
}

void AOWGenji::KillSuccess()
{
	//ToDo : 질풍참 초기화
}

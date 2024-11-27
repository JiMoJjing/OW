


#include "OWGenji.h"

#include "Components/CapsuleComponent.h"
#include "OW/ActorComponents/BasicAttack/Genji/GenjiShurikenComponent.h"
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

// Capsule Component Settings	
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
	BodyCollision->SetCapsuleHalfHeight(30.f);
	BodyCollision->SetCapsuleRadius(18.f);
	
	HeadCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	HeadCollision->SetRelativeLocation(FVector(0.f, -0.15f, 0.f));
	HeadCollision->SetCapsuleHalfHeight(12.f);
	HeadCollision->SetCapsuleRadius(12.f);

	UpperArmLCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	UpperArmLCollision->SetRelativeLocation(FVector(0.08f, 0.f, 0.f));
	UpperArmLCollision->SetCapsuleHalfHeight(18.f);
	UpperArmLCollision->SetCapsuleRadius(8.f);

	LowerArmLCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	LowerArmLCollision->SetCapsuleHalfHeight(16.f);
	LowerArmLCollision->SetCapsuleRadius(6.f);

	ThighLCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	ThighLCollision->SetCapsuleHalfHeight(26.f);
	ThighLCollision->SetCapsuleRadius(10.f);
	CalfLCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	CalfLCollision->SetCapsuleHalfHeight(26.f);
	CalfLCollision->SetCapsuleRadius(8.f);
	FootLCollision->SetRelativeLocation(FVector(0.08f, 0.f, 0.08f));
	FootLCollision->SetCapsuleHalfHeight(16.f);
	FootLCollision->SetCapsuleRadius(6.f);
	
	UpperArmRCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	UpperArmRCollision->SetRelativeLocation(FVector(-0.08f, 0.f, 0.f));
	UpperArmRCollision->SetCapsuleHalfHeight(18.f);
	UpperArmRCollision->SetCapsuleRadius(8.f);
	
	LowerArmRCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	LowerArmRCollision->SetCapsuleHalfHeight(16.f);
	LowerArmRCollision->SetCapsuleRadius(6.f);
	
	ThighRCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	ThighRCollision->SetCapsuleHalfHeight(26.f);
	ThighRCollision->SetCapsuleRadius(10.f);
	CalfRCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	CalfRCollision->SetCapsuleHalfHeight(26.f);
	CalfRCollision->SetCapsuleRadius(8.f);
	FootRCollision->SetRelativeLocation(FVector(-0.08f, 0.f, -0.08f));
	FootRCollision->SetCapsuleHalfHeight(16.f);
	FootRCollision->SetCapsuleRadius(6.f);
	
// Collision Profile Setting 임시
	GetCapsuleComponent()->SetCollisionProfileName(OWTEAM1CAPSULE);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	SetCollisionProfileName(OWTEAM1MESH);

// Create Actor Components	
	GenjiShurikenComponent = CreateDefaultSubobject<UGenjiShurikenComponent>(TEXT("ShurikenComponent"));

// For CharacterPlayable	
	BasicWeaponComponent = GenjiShurikenComponent;
}

void AOWGenji::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AOWGenji::BeginPlay()
{
	Super::BeginPlay();
}

void AOWGenji::PrimaryFire()
{
	if(GenjiShurikenComponent)
	{
		GenjiShurikenComponent->UseBasicWeapon(1.f);
	}
}

void AOWGenji::SecondaryFire()
{
	if(GenjiShurikenComponent)
	{
		GenjiShurikenComponent->UseAltFire(1.f);
	}
}

void AOWGenji::AbilityOne()
{
}

void AOWGenji::AbilityTwo()
{
}

void AOWGenji::AbilityThree()
{
}

void AOWGenji::QuickMelee()
{
}

void AOWGenji::Reload()
{
	GenjiShurikenComponent->UseReload(1.f);
}

void AOWGenji::InitWidget()
{
	Super::InitWidget();
	GenjiShurikenComponent->InitWidget();
}

void AOWGenji::KillSuccess()
{
	//ToDo : 질풍참 초기화
}

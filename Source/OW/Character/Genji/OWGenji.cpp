


#include "OWGenji.h"

#include "Components/CapsuleComponent.h"
#include "OW/ActorComponents/Ability/Genji/GenjiDeflectComponent.h"
#include "OW/ActorComponents/Ability/Genji/GenjiSwiftStrikeComponent.h"
#include "OW/ActorComponents/BasicAttack/QuickMeleeComponent.h"
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

// Create Actor Components	
	GenjiShurikenComponent = CreateDefaultSubobject<UGenjiShurikenComponent>(TEXT("ShurikenComponent"));

	QuickMeleeComponent = CreateDefaultSubobject<UQuickMeleeComponent>(TEXT("QuickMeleeComponent"));

	GenjiSwiftStrikeComponent = CreateDefaultSubobject<UGenjiSwiftStrikeComponent>(TEXT("SwiftStrikeComponent"));

	GenjiDeflectComponent = CreateDefaultSubobject<UGenjiDeflectComponent>(TEXT("DeflectComponent"));

// For CharacterPlayable	
	BasicWeaponComponent = GenjiShurikenComponent;


		
// Collision Profile Setting 임시
	GetCapsuleComponent()->SetCollisionProfileName(OWTEAM1CAPSULE);
	SetMeshCollisionProfileName(OWTEAM1MESH);
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
	if(GenjiSwiftStrikeComponent)
	{
		GenjiSwiftStrikeComponent->UseAbility();
	}
}

void AOWGenji::AbilityTwo()
{
	if(GenjiDeflectComponent)
	{
		GenjiDeflectComponent->UseAbility();
	}
}

void AOWGenji::AbilityThree()
{
}

void AOWGenji::QuickMelee()
{
	if(QuickMeleeComponent)
	{
		QuickMeleeComponent->UseQuickMelee(1.f);
	}
}

void AOWGenji::Reload()
{
	if(GenjiShurikenComponent)
	{
		GenjiShurikenComponent->UseReload(1.f);
	}
}

void AOWGenji::InitWidget()
{
	Super::InitWidget();
	GenjiShurikenComponent->InitWidget();
	GenjiSwiftStrikeComponent->InitWidget();
	GenjiDeflectComponent->InitWidget();
}

void AOWGenji::KillSuccess()
{
	//ToDo : 질풍참 초기화
	if(GenjiSwiftStrikeComponent)
	{
		GenjiSwiftStrikeComponent->SwiftStrikeCooldownReset();
	}
}

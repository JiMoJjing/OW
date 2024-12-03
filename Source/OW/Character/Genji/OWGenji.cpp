


#include "OWGenji.h"

#include "Components/CapsuleComponent.h"
#include "OW/ActorComponents/Ability/Genji/GenjiDeflectComponent.h"
#include "OW/ActorComponents/Ability/Genji/GenjiSwiftStrikeComponent.h"
#include "OW/ActorComponents/BasicAttack/QuickMeleeComponent.h"
#include "OW/ActorComponents/BasicAttack/Genji/GenjiShurikenComponent.h"
#include "OW/ActorComponents/Ultimate/Genji/GenjiDragonbladeComponent.h"
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

	GenjiDragonbladeComponent = CreateDefaultSubobject<UGenjiDragonbladeComponent>(TEXT("DragonbladeComponent"));

// For CharacterPlayable	
	BasicWeaponComponent = GenjiShurikenComponent;
}

void AOWGenji::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetCapsuleComponent()->SetCollisionProfileName(OWTEAM1CAPSULE);
	SetMeshCollisionProfileName(OWTEAM1MESH);
}

void AOWGenji::BeginPlay()
{
	Super::BeginPlay();
}

void AOWGenji::PrimaryFire()
{
	if(bUltimateActive && GenjiDragonbladeComponent)
	{
		GenjiDragonbladeComponent->DragonbladeSlash();
		return;
	}
	
	if(GenjiShurikenComponent)
	{
		GenjiShurikenComponent->UseBasicWeapon(1.f);
	}
}

void AOWGenji::SecondaryFire()
{
	if(bUltimateActive && GenjiDragonbladeComponent)
	{
		GenjiDragonbladeComponent->DragonbladeSlash();
		return;
	}
	
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
	if(GenjiDragonbladeComponent)
	{
		GenjiDragonbladeComponent->UseUltimate();
	}
}

void AOWGenji::QuickMelee()
{
	if(bUltimateActive && GenjiDragonbladeComponent)
	{
		GenjiDragonbladeComponent->DragonbladeSlash();
		return;
	}
	
	if(QuickMeleeComponent)
	{
		QuickMeleeComponent->UseQuickMelee(1.f);
	}
}

void AOWGenji::Reload()
{
	if(bUltimateActive)
	{
		return;
	}
	
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
	SwiftStrikeReset();
}

void AOWGenji::SwiftStrikeReset()
{
	if(GenjiSwiftStrikeComponent)
	{
		GenjiSwiftStrikeComponent->SwiftStrikeCooldownReset();
	}
}



#include "OWGenji.h"

#include "InputActionValue.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "OW/ActorComponents/Ability/Genji/GenjiDeflectComponent.h"
#include "OW/ActorComponents/Ability/Genji/GenjiSwiftStrikeComponent.h"
#include "OW/ActorComponents/BasicAttack/QuickMeleeComponent.h"
#include "OW/ActorComponents/BasicAttack/Genji/GenjiShurikenComponent.h"
#include "OW/ActorComponents/Ultimate/Genji/GenjiDragonbladeComponent.h"
#include "OW/Collision/OWCollisionProfile.h"

AOWGenji::AOWGenji() : bIsClimbWall(false), bCanDoubleJump(false), bCanClimbWall(true)
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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ClimbWallMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_ClimbWall.AM_Genji_ClimbWall'"));
	if(ClimbWallMontageRef.Object)
	{
		ClimbWallMontage = ClimbWallMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ClimbMantleMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_ClimbWall_Mantle.AM_Genji_ClimbWall_Mantle'"));
	if(ClimbMantleMontageRef.Object)
	{
		ClimbMantleMontage = ClimbMantleMontageRef.Object;
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

	DragonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GenjiDragon"));
	DragonMeshComponent->SetupAttachment(GetCapsuleComponent());
	DragonMeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	DragonMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	DragonMeshComponent->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GenjiDragonMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/OW_Genji_Assets/Dragonblade_Dragon/Dragonblade_Dragon.Dragonblade_Dragon'"));
	if(GenjiDragonMeshRef.Object)
	{
		DragonMeshComponent->SetSkeletalMesh(GenjiDragonMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> DragonAnimInstanceRef(TEXT("/Game/OW/Genji/AnimInstance/ABP_Dragon.ABP_Dragon_C"));
	if(DragonAnimInstanceRef.Class)
	{
		DragonMeshComponent->SetAnimInstanceClass(DragonAnimInstanceRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DragonMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW_Genji_Assets/Dragonblade_Dragon/Animations/AM_Dragonblade.AM_Dragonblade'"));
	if(DragonMontageRef.Object)
	{
		DragonMontage = DragonMontageRef.Object;
	}
	
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

void AOWGenji::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	
	if(bIsJumpPressed)
	{
		FHitResult TraceHit;
		FVector TraceStartLocation = GetActorLocation() + FVector(0.f, 0.f, 90.f);
		FVector TraceEndLocation = TraceStartLocation + GetActorForwardVector() * 50.f;
		
		if(GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStartLocation, TraceEndLocation, ECC_Camera))
		{
			if(FVector::DotProduct(TraceHit.Normal, GetActorForwardVector()) < -0.75f)
			{
				if(bCanClimbWall)
				{
					ClimbWallStart(TraceHit.Normal);
				}
			}
		}
		else
		{
			if(bIsClimbWall)
			{
				StopClimbing();
			}
		}
	}

	if(bIsClimbWall)
	{
		const FVector ClimbDir = GetActorForwardVector() + GetActorUpVector();
		AddMovementInput(ClimbDir, 1.f);
		return;
	}

	const FRotator YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InputValue.X);
	AddMovementInput(RightDirection, InputValue.Y);
	 
}

void AOWGenji::MoveReleased()
{
	if(bIsClimbWall)
	{
		StopClimbing();
	}
}

void AOWGenji::JumpPressed()
{
	bIsJumpPressed = true;
}

void AOWGenji::JumpReleased()
{
	bIsJumpPressed = false;

	if(bIsClimbWall)
	{
		StopClimbing();
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
	OnDragonbladeStart();
}

void AOWGenji::OnDragonbladeStart()
{
	if(GenjiSwiftStrikeComponent)
	{
		GenjiSwiftStrikeComponent->SwiftStrikeCooldownReset();
	}
	if(GenjiShurikenComponent)
	{
		GenjiShurikenComponent->OnDragonbladeStart();
	}
}

void AOWGenji::DragonMontageStart()
{
	if(DragonMontage && DragonMeshComponent)
	{
		DragonMeshComponent->SetVisibility(true);
		FOnMontageEnded MontageEnded;
		MontageEnded.BindLambda(
			[&](UAnimMontage* Montage, bool bInterrupted)
			{
				DragonMeshComponent->SetVisibility(false);
			});
		DragonMeshComponent->GetAnimInstance()->Montage_Play(DragonMontage);
		DragonMeshComponent->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded, DragonMontage);
	}
}

void AOWGenji::ClimbWallStart(FVector& HitNormal)
{
	bIsClimbWall = true;
	bCanDoubleJump = true;
	bCanClimbWall = false;

	if(JumpCurrentCount >= 2)
	{
		bCanDoubleJump = false;
	}

	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->MaxAcceleration = 10000.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	FVector ClimbDir = -HitNormal;
	ClimbDir.Z = 0.f;
	FRotator ClimbRot = FRotationMatrix::MakeFromX(ClimbDir).Rotator();
	SetActorRotation(ClimbRot);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		FOnMontageEnded MontageEnded;
		MontageEnded.BindLambda(
			[&](UAnimMontage* Montage, bool bInterrupted)
			{
				if(Montage == ClimbWallMontage)
				{
					ClimbWallEnd();
				}
			});
		AnimInstance->Montage_Play(ClimbWallMontage);
		AnimInstance->Montage_SetEndDelegate(MontageEnded);
	}
}

void AOWGenji::ClimbWallEnd()
{
	bIsClimbWall = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	GetCharacterMovement()->MaxAcceleration = 2048.f;
	GetCharacterMovement()->MaxFlySpeed = 5000.f;
	bUseControllerRotationYaw = true;

	//GetMesh()->GetAnimInstance()->Montage_Play(ClimbMantleMontage);
}

void AOWGenji::StopClimbing()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance->Montage_IsActive(ClimbWallMontage))
	{
		AnimInstance->Montage_Stop(0.f, ClimbWallMontage);
	}
}

void AOWGenji::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bCanClimbWall = true;
}

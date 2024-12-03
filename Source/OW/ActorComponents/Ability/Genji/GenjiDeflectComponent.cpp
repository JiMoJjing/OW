// Fill out your copyright notice in the Description page of Project Settings.


#include "GenjiDeflectComponent.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "OW/Collision/OWCollisionProfile.h"
#include "OW/Character/OWCharacterPlayable.h"
#include "OW/Character/Genji/OWGenji.h"
#include "OW/Projectile/OWProjectileBase.h"
#include "OW/Interface/OWPlayerTraceInterface.h"


UGenjiDeflectComponent::UGenjiDeflectComponent() : DeflectMontageSectionIndex(0)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_Deflect.AM_Genji_Deflect'"));
	if(DeflectMontageRef.Object)
	{
		AbilityMontage = DeflectMontageRef.Object;
	}
	
	DeflectBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DeflectBoxComponent"));
	DeflectBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &UGenjiDeflectComponent::OnDeflectBoxBeginOverlap);
	DeflectBoxComponent->SetCollisionProfileName(OWTEAM1OVERLAP);
	DeflectBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DeflectBoxComponent->SetBoxExtent(FVector(64.f, 96.f, 96.f));

	DeflectMontageSection.Add(0, TEXT("Section_00"));
	DeflectMontageSection.Add(1, TEXT("Section_01"));
	DeflectMontageSection.Add(2, TEXT("Section_02"));
	DeflectMontageSection.Add(3, TEXT("Section_03"));

	DeflectMontageSection.Add(4, TEXT("Section_04"));
	DeflectMontageSection.Add(5, TEXT("Section_05"));
	DeflectMontageSection.Add(6, TEXT("Section_06"));
	DeflectMontageSection.Add(7, TEXT("Section_07"));

	CooldownTime = 8.f;
	DurationTime = 2.f;

	AbilityType = EAbilityType::EAT_AbilityTwo;
}

void UGenjiDeflectComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	if(IOWPlayerTraceInterface* OWPlayerTraceInterface = Cast<IOWPlayerTraceInterface>(GetOwner()))
	{
		PlayerTraceInterface = TScriptInterface<IOWPlayerTraceInterface>(GetOwner());
	}

	if(DeflectBoxComponent)
	{
		DeflectBoxComponent->AttachToComponent(CharacterPlayable->GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DeflectBoxComponent->SetRelativeLocation(FVector(64.f, 0.f, 0.f));
	}

	if(AOWGenji* Genji = Cast<AOWGenji>(GetOwner()))
	{
		GenjiRef = Genji;
	}
}

void UGenjiDeflectComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGenjiDeflectComponent::AbilityStart()
{
	Super::AbilityStart();

	DeflectStartSetting();

	DurationStart();
}

void UGenjiDeflectComponent::AbilityEnd()
{
	Super::AbilityEnd();

	DeflectEndSetting();

	CooldownStart();
}

void UGenjiDeflectComponent::OtherAbilityStart(EAbilityType OtherAbilityType)
{
	Super::OtherAbilityStart(OtherAbilityType);

	if(AbilityState == EAbilityState::EAS_Active)
	{
		DurationEnd();
	}
}

void UGenjiDeflectComponent::DeflectStartSetting()
{
	// Collision 활성화
	DeflectBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UGenjiDeflectComponent::DeflectEndSetting()
{
	// 콜리전 비활성화
	DeflectBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UGenjiDeflectComponent::GetDeflectDirection(const FVector& InStartLocation, FVector& InDirectionVector)
{
	PlayerTraceInterface->GetDirectionToCrosshair(InStartLocation, InDirectionVector, OWTEAM1TRACE);
}

void UGenjiDeflectComponent::DurationStart()
{
	GetOwner()->GetWorldTimerManager().SetTimer(DurationTimerHandle, this, &UGenjiDeflectComponent::DurationEnd, DurationTime, false);
}

void UGenjiDeflectComponent::DurationEnd()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(DurationTimerHandle);
	AbilityEnd();
}

void UGenjiDeflectComponent::OnDeflectBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AOWProjectileBase* OverlappedProjectile = Cast<AOWProjectileBase>(OtherComp->GetOwner()))
	{		
		UClass* ProjectileClass = OverlappedProjectile->GetClass();
		OverlappedProjectile->DeactivateProjectile();
		

		FTransform SpawnTransform;
		FVector DirectionVector;
		FVector StartLocation = OverlappedProjectile->GetActorLocation();
		GetDeflectDirection(StartLocation, DirectionVector);
		
		AOWProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AOWProjectileBase>(ProjectileClass, SpawnTransform, CharacterPlayable, CharacterPlayable);
		SpawnedProjectile->SetCollisionProfile(OWTEAM1HIT);
		SpawnedProjectile->FinishSpawning(SpawnedProjectile->GetTransform(), true);
		SpawnedProjectile->ActivateProjectile(StartLocation, DirectionVector);

		if(GenjiRef->IsUltimateActive())
		{
			PlayAbilityMontage_JumpToSection(AbilityMontage, DeflectMontageSection[DeflectMontageSectionIndex++ + 4]);
		}
		else
		{
			PlayAbilityMontage_JumpToSection(AbilityMontage, DeflectMontageSection[DeflectMontageSectionIndex++]);
		}
		DeflectMontageSectionIndex %= 4;
	}
}

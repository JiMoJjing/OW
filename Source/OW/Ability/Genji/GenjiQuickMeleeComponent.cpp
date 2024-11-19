// Fill out your copyright notice in the Description page of Project Settings.


#include "GenjiQuickMeleeComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OW/Character/OWCharacterPlayable.h"
#include "OW/Collision/OWCollisionProfile.h"


UGenjiQuickMeleeComponent::UGenjiQuickMeleeComponent() : QuickMeleeDamage(40.f)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AbilityMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_QuickMelee.AM_Genji_QuickMelee'"));
	if(AbilityMontageRef.Object)
	{
		AbilityMontage = AbilityMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> QuickMeleeEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/OW/Genji/Niagara/NS_Genji_QuickMeleeHit.NS_Genji_QuickMeleeHit'"));
	if(QuickMeleeEffectRef.Object)
	{
		QuickMeleeEffect = QuickMeleeEffectRef.Object;
	}
	
	QuickMeleeCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("QuickMeleeCollider"));
	QuickMeleeCollider->OnComponentBeginOverlap.AddDynamic(this, &UGenjiQuickMeleeComponent::OnBoxBeginOverlap);
	QuickMeleeCollider->SetBoxExtent(FVector(75.f, 64.f, 32.f));
	QuickMeleeCollider->SetCollisionProfileName(OWTEAM1OVERLAP);
	QuickMeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AbilityType = EAbilityType::EAT_QuickMelee;
	CooldownTime = 1.f;
}

void UGenjiQuickMeleeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	QuickMeleeCollider->AttachToComponent(PlayableCharacter->GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	QuickMeleeCollider->SetRelativeLocation(FVector(75.f, 0.f, 0.f));
}

void UGenjiQuickMeleeComponent::AbilityStart()
{
	Super::AbilityStart();

	PlayableCharacter->OnAnimNotifyBegin.AddUObject(this, &UGenjiQuickMeleeComponent::QuickMeleeBegin);
	PlayableCharacter->OnAnimNotifyEnd.AddUObject(this, &UGenjiQuickMeleeComponent::QuickMeleeEnd);

	OverlappedActors.Empty();

	PlayAbilityMontage();
}

void UGenjiQuickMeleeComponent::AbilityEnd()
{
	Super::AbilityEnd();

	PlayableCharacter->OnAnimNotifyBegin.RemoveAll(this);
	PlayableCharacter->OnAnimNotifyEnd.RemoveAll(this);

	CooldownStart();
}

void UGenjiQuickMeleeComponent::QuickMeleeBegin()
{
	QuickMeleeCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UGenjiQuickMeleeComponent::QuickMeleeEnd()
{
	QuickMeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AbilityEnd();
}

void UGenjiQuickMeleeComponent::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AActor* OverlappedActor = OtherComp->GetOwner())
	{
		if(OverlappedActors.Find(OverlappedActor) != INDEX_NONE)
		{
			return;
		}
		OverlappedActors.AddUnique(OverlappedActor);
		UGameplayStatics::ApplyDamage(OverlappedActor, QuickMeleeDamage, PlayableCharacter->GetController(), GetOwner(), UDamageType::StaticClass());

		const FVector EffectLocation = OtherComp->GetComponentLocation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(OtherComp, QuickMeleeEffect, EffectLocation, FRotator::ZeroRotator);
	}
}

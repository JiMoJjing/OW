// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickMeleeComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OW/Character/OWCharacterPlayable.h"
#include "OW/Collision/OWCollisionProfile.h"


UQuickMeleeComponent::UQuickMeleeComponent() : DelayTime(1.f), QuickMeleeDamage(40.f)
{
	bWantsInitializeComponent = true;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> QuickMeleeMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_QuickMelee.AM_Genji_QuickMelee'"));
	if(QuickMeleeMontageRef.Object)
	{
		QuickMeleeMontage = QuickMeleeMontageRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> QuickMeleeEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/OW/Genji/Niagara/NS_Genji_QuickMeleeHit.NS_Genji_QuickMeleeHit'"));
	if(QuickMeleeEffectRef.Object)
	{
		QuickMeleeEffect = QuickMeleeEffectRef.Object;
	}
	
	QuickMeleeBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("QuickMeleeCollider"));
	QuickMeleeBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &UQuickMeleeComponent::OnBoxBeginOverlap);
	QuickMeleeBoxComponent->SetBoxExtent(FVector(75.f, 64.f, 32.f));
	QuickMeleeBoxComponent->SetCollisionProfileName(OWTEAM1OVERLAP);
	QuickMeleeBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void UQuickMeleeComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	if(AOWCharacterPlayable* OwnerCharacter = Cast<AOWCharacterPlayable>(GetOwner()))
	{
		CharacterPlayable = OwnerCharacter;
	}

	if(IOWTriggerAnimNotifyInterface* AnimNotifyInterface = Cast<IOWTriggerAnimNotifyInterface>(GetOwner()))
	{
		TriggerAnimNotifyInterface = TScriptInterface<IOWTriggerAnimNotifyInterface>(GetOwner());
	}
	
	if(QuickMeleeBoxComponent)
	{
		QuickMeleeBoxComponent->AttachToComponent(CharacterPlayable->GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		QuickMeleeBoxComponent->SetRelativeLocation(FVector(75.f, 0.f, 0.f));
	}
}

void UQuickMeleeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UQuickMeleeComponent::UseQuickMelee(float InPlayRate)
{
	if(CheckAvailable())
	{
		CharacterPlayable->OnAnimNotifyBegin.AddUObject(this, &UQuickMeleeComponent::QuickMeleeBegin);
		CharacterPlayable->OnAnimNotifyEnd.AddUObject(this, &UQuickMeleeComponent::QuickMeleeEnd);
		PlayQuickMeleeMontage(InPlayRate);
		DelayStart(DelayTime);
	}
}

bool UQuickMeleeComponent::CheckAvailable()
{
	bool bAvailable = true;
	// Check CharacterPlayable Ability Not Active
	bAvailable &= !bDelayActive;

	return bAvailable;
}

void UQuickMeleeComponent::PlayQuickMeleeMontage(float InPlayRate)
{
	if(QuickMeleeMontage)
	{
		CharacterPlayable->GetMesh()->GetAnimInstance()->Montage_Play(QuickMeleeMontage, InPlayRate);
	}
}

void UQuickMeleeComponent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage == QuickMeleeMontage)
	{
		CharacterPlayable->OnAnimNotifyBegin.RemoveAll(this);
		CharacterPlayable->OnAnimNotifyEnd.RemoveAll(this);
	}
}

void UQuickMeleeComponent::DelayStart(float InDelayTime)
{
	bDelayActive = true;
	CharacterPlayable->SetQuickMeleeActive(true);
	CharacterPlayable->GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &UQuickMeleeComponent::DelayEnd, InDelayTime, false);
}

void UQuickMeleeComponent::DelayEnd()
{
	CharacterPlayable->SetQuickMeleeActive(false);
	bDelayActive = false;
}

void UQuickMeleeComponent::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AActor* OverlappedActor = OtherComp->GetOwner())
	{
		if(OverlappedActors.Find(OverlappedActor) != INDEX_NONE)
		{
			return;
		}
		OverlappedActors.AddUnique(OverlappedActor);
		UGameplayStatics::ApplyDamage(OverlappedActor, QuickMeleeDamage, CharacterPlayable->GetController(), GetOwner(), UDamageType::StaticClass());

		const FVector EffectLocation = OtherComp->GetComponentLocation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(OtherComp, QuickMeleeEffect, EffectLocation, FRotator::ZeroRotator);
	}
}

void UQuickMeleeComponent::QuickMeleeBegin()
{
	OverlappedActors.Empty();
	QuickMeleeBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UQuickMeleeComponent::QuickMeleeEnd()
{
	QuickMeleeBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GenjiDragonbladeComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "OW/Character/OWCharacterPlayable.h"
#include "OW/Character/Genji/OWGenji.h"
#include "OW/Collision/OWCollisionProfile.h"

UGenjiDragonbladeComponent::UGenjiDragonbladeComponent() : DragonbladeDuration(6.f), DragonbladeSlashIndex(0), DragonbladeSlashDelayTime(0.7f), bDragonbladeSlashDelay(false)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UltimateMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_NormaltoDragonblade.AM_Genji_NormaltoDragonblade'"));
	if(UltimateMontageRef.Object)
	{
		UltimateMontage = UltimateMontageRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DragonbladeToNormalMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Genji_DragonbladetoNormal.AM_Genji_DragonbladetoNormal'"));
	if(DragonbladeToNormalMontageRef.Object)
	{
		DragonbladeToNormalMontage = DragonbladeToNormalMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DragonbladeSlashMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OW/Genji/AnimMontages/AM_Dragonblade_Slash.AM_Dragonblade_Slash'"));
	if(DragonbladeSlashMontageRef.Object)
	{
		DragonbladeSlashMontage = DragonbladeSlashMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> DragonbladeSlashHitEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/OW/Genji/Niagara/NS_Genji_DragonbladeSlashHit.NS_Genji_DragonbladeSlashHit'"));
	if(DragonbladeSlashHitEffectRef.Object)
	{
		DragonbladeSlashHitEffect = DragonbladeSlashHitEffectRef.Object;
	}
	
	DragonbladeSlashSections.Add(0, TEXT("Section_00"));
	DragonbladeSlashSections.Add(1, TEXT("Section_01"));
}

void UGenjiDragonbladeComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if(AOWGenji* Genji = Cast<AOWGenji>(GetOwner()))
	{
		GenjiRef = Genji;
	}
}

void UGenjiDragonbladeComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGenjiDragonbladeComponent::StartUltimate()
{
	Super::StartUltimate();
	CharacterPlayable->OnAnimNotifyEnd.AddUObject(this, &UGenjiDragonbladeComponent::EndUltimate);
	PlayUltimateMontage(UltimateMontage);
}

void UGenjiDragonbladeComponent::EndUltimate()
{
	Super::EndUltimate();
	CharacterPlayable->OnAnimNotifyEnd.RemoveAll(this);
	DragonbladeDurationStart();
}

void UGenjiDragonbladeComponent::DragonbladeDurationStart()
{
	GenjiRef->SwiftStrikeReset();
	CharacterPlayable->GetWorldTimerManager().SetTimer(DragonbladeDurationTimerHandle, this, &UGenjiDragonbladeComponent::DragonbladeDurationEnd, DragonbladeDuration, false);
	CharacterPlayable->SetMaxWalkSpeedByMultiplier(1.3f);
}

void UGenjiDragonbladeComponent::DragonbladeDurationEnd()
{
	UAnimInstance* AnimInstance = CharacterPlayable->GetMesh()->GetAnimInstance();
	if(AnimInstance->IsAnyMontagePlaying())
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &UGenjiDragonbladeComponent::DragonbladeDurationDelayEnd);
		return;
	}
	CharacterPlayable->SetUltimateActive(false);
	CharacterPlayable->OnAnimNotifyEnd.AddUObject(this, &UGenjiDragonbladeComponent::DragonbladeToNormal);
	CharacterPlayable->AbilityStart(EAbilityType::EAT_AbilityThree);
	CharacterPlayable->SetMaxWalkSpeedToDefault();
	PlayUltimateMontage(DragonbladeToNormalMontage);
}

void UGenjiDragonbladeComponent::DragonbladeDurationDelayEnd(UAnimMontage* Montage, bool bInterrupted)
{
	UAnimInstance* AnimInstance = CharacterPlayable->GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->OnMontageEnded.RemoveAll(this);
	}

	CharacterPlayable->SetUltimateActive(false);
	CharacterPlayable->OnAnimNotifyEnd.AddUObject(this, &UGenjiDragonbladeComponent::DragonbladeToNormal);
	CharacterPlayable->AbilityStart(EAbilityType::EAT_AbilityThree);
	CharacterPlayable->SetMaxWalkSpeedToDefault();
	PlayUltimateMontage(DragonbladeToNormalMontage);
}

void UGenjiDragonbladeComponent::DragonbladeToNormal()
{
	CharacterPlayable->AbilityEnd(EAbilityType::EAT_AbilityThree);
	CharacterPlayable->OnAnimNotifyEnd.RemoveAll(this);
}

void UGenjiDragonbladeComponent::DragonbladeSlash()
{
	if(CharacterPlayable->GetCurrentAbilityType() == EAbilityType::EAT_None && !bDragonbladeSlashDelay)
	{
		FName SectionName = DragonbladeSlashSections[DragonbladeSlashIndex++];
		DragonbladeSlashIndex %= 2;
		
		CharacterPlayable->GetMesh()->GetAnimInstance()->Montage_Play(DragonbladeSlashMontage);
		CharacterPlayable->GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, DragonbladeSlashMontage);

		DragonbladeSlashTrace();
		DragonbladeSlashDelayStart();
	}
}

void UGenjiDragonbladeComponent::DragonbladeSlashTrace()
{
	FVector ForwardVector = GetOwner()->GetActorForwardVector();
	FVector StartLocation = GetOwner()->GetActorLocation() + ForwardVector * 200.f;
	FRotator ControlRotation = CharacterPlayable->GetControlRotation();
	TArray<FHitResult> HitResults;

#if WITH_EDITOR
	DrawDebugBox(GetWorld(), StartLocation, FVector(200.f, 200.f, 72.f), FQuat::MakeFromRotator(ControlRotation),  FColor::Green, false, 2.f);
#endif
	GetWorld()->SweepMultiByChannel(HitResults, StartLocation, StartLocation, FQuat::MakeFromRotator(ControlRotation), OWTEAM1TRACE, FCollisionShape::MakeBox(FVector(200.f, 200.f, 72.f)));

	TArray<AActor*> HitActors;
	for(auto& HitResult : HitResults)
	{
		if(AOWCharacterBase* HitCharacter = Cast<AOWCharacterBase>(HitResult.GetActor()))
		{
			HitActors.AddUnique(HitCharacter);
		}
	}

	for(auto& HitActor : HitActors)
	{
		UGameplayStatics::ApplyDamage(HitActor, 110.f, CharacterPlayable->GetController(), CharacterPlayable, UDamageType::StaticClass());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(HitActor, DragonbladeSlashHitEffect, HitActor->GetActorLocation());
	}
}

void UGenjiDragonbladeComponent::DragonbladeSlashDelayStart()
{
	bDragonbladeSlashDelay = true;
	CharacterPlayable->GetWorldTimerManager().SetTimer(DragonbladeSlashDelayTimerHandle, this, &UGenjiDragonbladeComponent::DragonbladeSlashDelayEnd, DragonbladeSlashDelayTime, false);
}

void UGenjiDragonbladeComponent::DragonbladeSlashDelayEnd()
{
	bDragonbladeSlashDelay = false;
}
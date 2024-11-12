


#include "OWGenji.h"

#include "Components/CapsuleComponent.h"

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
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.f);
	GetCapsuleComponent()->SetCapsuleRadius(30.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	JumpMaxCount = 2;
}

void AOWGenji::BeginPlay()
{
	Super::BeginPlay();
}

void AOWGenji::KillSuccess()
{
	//ToDo : 질풍참 초기화
}

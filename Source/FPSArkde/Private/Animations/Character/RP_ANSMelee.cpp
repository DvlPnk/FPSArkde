// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Character/RP_ANSMelee.h"
#include "RP_CharacterPlayer.h"

void URP_ANSMelee::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* CharacterActor =  MeshComp->GetOwner();
	if(IsValid(CharacterActor))
	{
		ARP_CharacterPlayer* Character = Cast<ARP_CharacterPlayer>(CharacterActor);
		if(IsValid(Character))
		{
			Character->SetMeleeDetectorCollision(ECollisionEnabled::QueryOnly);
		}
	}
}

void URP_ANSMelee::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		ARP_CharacterPlayer* Character = Cast<ARP_CharacterPlayer>(CharacterActor);
		if (IsValid(Character))
		{
			Character->SetMeleeDetectorCollision(ECollisionEnabled::NoCollision);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Character/RP_ANEnableAction.h"
#include "RP_CharacterPlayer.h"

void URP_ANEnableAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		ARP_CharacterPlayer* Character = Cast<ARP_CharacterPlayer>(CharacterActor);
		if (IsValid(Character))
		{
			Character->SetMeleeState(false);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RP_HealthBox.h"

#include "RP_CharacterPlayer.h"

ARP_HealthBox::ARP_HealthBox()
{
	HealthBoxMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthBoxMeshComponent"));
	HealthBoxMeshComponent->SetupAttachment(RootComponent);
	HealthBoxMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthValue = 15.0f;
}

void ARP_HealthBox::PickUp(ARP_CharacterPlayer* PickUpCharacter)
{
	Super::PickUp(PickUpCharacter);

	bool bSuccessfulHealt = PickUpCharacter->TryAddHealth(HealthValue);

	if(bSuccessfulHealt)
	{
		Destroy();
	}
}

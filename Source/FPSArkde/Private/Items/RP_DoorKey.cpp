// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RP_DoorKey.h"
#include "Components/StaticMeshComponent.h"
#include "RP_CharacterPlayer.h"
;
ARP_DoorKey::ARP_DoorKey()
{
	KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMeshComponent"));
	KeyMeshComponent->SetupAttachment(RootComponent);
	KeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	XPValue = 40.0f;
	KeyTag = "Key_A";
}

void ARP_DoorKey::PickUp(ARP_CharacterPlayer* PickUpCharacter)
{
	Super::PickUp(PickUpCharacter);

	PickUpCharacter->AddKey(KeyTag);
	Destroy();
}

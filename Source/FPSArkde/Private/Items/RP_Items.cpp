// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RP_Items.h"
#include "Components/SphereComponent.h"
#include "RP_CharacterPlayer.h"

// Sets default values
ARP_Items::ARP_Items()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MainColliderComponent"));
	MainColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = MainColliderComponent;
}

// Called when the game starts or when spawned
void ARP_Items::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARP_Items::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARP_Items::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (IsValid(OtherActor))
	{
		ARP_CharacterPlayer* OverlappedCharacter = Cast<ARP_CharacterPlayer>(OtherActor);
		if (IsValid(OverlappedCharacter)) {
			PickUp(OverlappedCharacter);
		}
	}

}

void ARP_Items::PickUp(ARP_CharacterPlayer* PickUpCharacter)
{
	BP_PickUp(PickUpCharacter);
}

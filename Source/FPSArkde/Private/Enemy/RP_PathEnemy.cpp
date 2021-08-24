// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RP_PathEnemy.h"

#include "Components/BillboardComponent.h"

// Sets default values
ARP_PathEnemy::ARP_PathEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PathBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("PathBillBoard"));
	RootComponent = PathBillboardComponent;

}

// Called when the game starts or when spawned
void ARP_PathEnemy::BeginPlay()
{
	Super::BeginPlay();
	
} 


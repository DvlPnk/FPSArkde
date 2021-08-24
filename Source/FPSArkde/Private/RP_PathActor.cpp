// Fill out your copyright notice in the Description page of Project Settings.


#include "RP_PathActor.h"

// Sets default values
ARP_PathActor::ARP_PathActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARP_PathActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARP_PathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


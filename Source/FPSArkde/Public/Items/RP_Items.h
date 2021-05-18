// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RP_Items.generated.h"

class USphereComponent;
class ARP_CharacterPlayer;

UCLASS()
class FPSARKDE_API ARP_Items : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* MainColliderComponent;


public:	
	// Sets default values for this actor's properties
	ARP_Items();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PickUp(ARP_CharacterPlayer* PickUpCharacter);

	UFUNCTION(BlueprintImplementableEvent ,BlueprintCallable, Category = "Item")
	void BP_PickUp(ARP_CharacterPlayer* PickUpCharacter);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};

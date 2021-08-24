// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/RP_Items.h"
#include "RP_DoorKey.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class FPSARKDE_API ARP_DoorKey : public ARP_Items
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* KeyMeshComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	FName KeyTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	float XPValue;

public:

	ARP_DoorKey();

public:

	UFUNCTION(BlueprintCallable, Category = "Getter")
	FName GetKeyTag() const { return KeyTag; };

protected:

	virtual void PickUp(ARP_CharacterPlayer* PickUpCharacter) override;

};

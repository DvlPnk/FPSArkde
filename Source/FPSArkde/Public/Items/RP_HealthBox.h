// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/RP_Items.h"
#include "RP_HealthBox.generated.h"

/**
 * 
 */
UCLASS()
class FPSARKDE_API ARP_HealthBox : public ARP_Items
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HealthBoxMeshComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Box")
	float HealthValue;

public:

	ARP_HealthBox();

public:

	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetHealthValue() const { return HealthValue; };

protected:

	virtual void PickUp(ARP_CharacterPlayer* PickUpCharacter) override;
	
};

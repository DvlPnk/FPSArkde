// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/RP_Weapon.h"
#include "RP_Shotgun.generated.h"

class UParticleSystem;
/**
 * 
 */
UCLASS()
class FPSARKDE_API ARP_Shotgun : public ARP_Weapon
{
	GENERATED_BODY()

public:

	ARP_Shotgun();

protected:

	virtual void StartAction() override;

	virtual void StopAction() override;
};

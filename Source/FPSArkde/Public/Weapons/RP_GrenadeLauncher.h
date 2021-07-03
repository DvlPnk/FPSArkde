// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/RP_Weapon.h"
#include "RP_GrenadeLauncher.generated.h"

class ARP_Projectile;
/**
 * 
 */
UCLASS()
class FPSARKDE_API ARP_GrenadeLauncher : public ARP_Weapon
{
	GENERATED_BODY()


public:

	ARP_GrenadeLauncher();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GranadeLauncher")
	TSubclassOf<ARP_Projectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName MuzzleSocketName;
	
protected:

	virtual void StartAction() override;

	virtual void StopAction() override;
};

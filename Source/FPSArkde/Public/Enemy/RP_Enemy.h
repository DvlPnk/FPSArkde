// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RP_CharacterPlayer.h"
#include "RP_Enemy.generated.h"

class URP_EnemyHealthBar;
class ARP_AIController;
class ARP_Items;
class ARP_PathEnemy;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class FPSARKDE_API ARP_Enemy : public ARP_CharacterPlayer
{
	GENERATED_BODY()

public:
	
	ARP_Enemy();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* WidgetHealthBarComponent;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navegation Path")
	bool bLoopPath;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	bool bIsShowingHealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navegation Path")
	int DirectionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navegation Path")
	float WaitingTimeOnPathPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Ultimate XP")
	float XPValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	float LootProbability;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Navegation Path")
	ARP_PathEnemy* MyPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	TSubclassOf<ARP_Items> LootItemClass;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Controller")
	ARP_AIController* MyAIController;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	URP_EnemyHealthBar* EnemyHealthBar;

	FTimerHandle TimerHandle_HideHealthBar;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:

	UFUNCTION()
	void GiveXP(AActor* DamageCauser);

	UFUNCTION()
	void HealthChange(URP_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_GiveXP(AActor* DamageCauser);

	bool TrySpawnLoot();

public:

	bool GetLoopPath() { return bLoopPath; };

	int GetDirectionIndex() { return DirectionIndex; };
	
	float GetWatingTime() { return WaitingTimeOnPathPoint; };

	void ShowHealthBar();
	void HideHealthBar();
	
};

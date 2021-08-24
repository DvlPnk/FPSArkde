// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RP_BotSpawner.generated.h"

class ARP_Bot;
class UBillBoardComponent;

UCLASS()
class FPSARKDE_API ARP_BotSpawner : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* SpawnerBillboardComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (UIMin = 1.0, ClampMin = 1.0))
	int MaxBotCounter;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
	int CurrentBotCounter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float TimeToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (MakeEditWidget = true))
	TArray<FVector> SpawnPoints;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner")
	TSubclassOf<ARP_Bot> BotClass;

	FTimerHandle TimerHandle_SpawnBot;
	
public:	
	// Sets default values for this actor's properties
	ARP_BotSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnBot();

	FVector GetSpawnPoint();

public:

	void NotifyBotDead();
};

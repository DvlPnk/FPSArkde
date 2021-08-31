// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RP_Bot.generated.h"

class USoundCue;
class URP_GameInstance;
class ARP_Items;
class USphereComponent;
class URP_HealthComponent;
class ARP_CharacterPlayer;
class UStaticMeshComponent;
class ARP_BotSpawner;
class UAudioComponent;

UCLASS()
class FPSARKDE_API ARP_Bot : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BotMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SelfDestructionDetectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URP_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* TimerSoundComponent;
	

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot|Debug")
	bool bDebug;
	
	UPROPERTY(BlueprintReadOnly, Category = "Bot|Self Destruction")
	bool bIsExploded;

	UPROPERTY(BlueprintReadOnly, Category = "Bot|Movement")
	bool bIsStartingCountdown;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot|Movement")
	float MinDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot|Movement")
	float ForceMagnitude;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot|Effect")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot|Effect")
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bot|Ultimate XP")
	float XPValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	float LootProbability;
	
	UPROPERTY(BlueprintReadOnly, Category = "Bot")
	FVector NextPathPoint;
	
	UPROPERTY(BlueprintReadOnly, Category = "References")
	ARP_CharacterPlayer* PlayerCharacter;

	URP_GameInstance* GameInstanceReference;

	UMaterialInstanceDynamic* BotMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot|Effect")
	UParticleSystem* ExplosionEffect;

	FTimerHandle TimerHandle_SelfDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	TSubclassOf<ARP_Items> LootItemClass;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
	ARP_BotSpawner* MySpawner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* ExplosionSound;
	
public:
	// Sets default values for this pawn's properties
	ARP_Bot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Bot|Navigation")
	FVector GetNextPathPoint();

	UFUNCTION()
	void TakingDamage(URP_HealthComponent* CurrrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void SelfDestruction();

	UFUNCTION()
	void StartCountdown(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SelfDamage();

	UFUNCTION()
	void GiveXP(AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_GiveXP(AActor* DamageCauser);

	bool TrySpawnLoot();

	void PlayTimerSound();

	void PlayExplosionSound();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSpawner(ARP_BotSpawner* NewSpawner) { MySpawner = NewSpawner; };
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RP_CharacterPlayer.generated.h"

class URP_GameInstance;
class ARP_GameMode;
class USpringArmComponent;
class UCameraComponent;
class ARP_Weapon;
class UAnimMontage;
class UAnimInstance;
class URP_HealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUltimateUpdateSignature, float, CurrentUltimateXP, float, MaxUltimateXP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUltimateStatusSignature, bool, bIsAvailable);

UENUM(Blueprintable)
enum class ERP_CharacterType : uint8
{
	CharacterType_Player		UMETA(DisplayName = "Player"),
	CharacterType_Enemy			UMETA(DisplayName = "Enemy"),
};

UCLASS()
class FPSARKDE_API ARP_CharacterPlayer : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComponent_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* TPSCameraComponent_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componets")
		UCapsuleComponent* MeleeDetectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		URP_HealthComponent* HealthComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
		bool bUseFirstPersonView;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
		bool bUseTPersonView_R;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
		bool bIsLookInversion;

	UPROPERTY(BlueprintReadOnly, Category = "Melee")
		bool bIsDoingMelee;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		bool bCanUseWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		bool bCanMakeCombos;

	UPROPERTY(BlueprintReadOnly, Category = "Melee")
		bool bIsComboEnable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Over")
		bool bHasToDestroy;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
		bool bCanUseUltimate;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
		bool bIsUsingUltimate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate")
		bool bUltimateWithTicks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		float MeleeDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
		float MaxComboMultiplier;

	UPROPERTY(BlueprintReadOnly, Category = "Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
		float CurrentComboMultipier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate", meta = (ClampMin = 0.0, UIMin = 0.0))
		float MaxUltimateXP;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
		float CurrentUltimateXP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Time", meta = (ClampMin = 0.0, UIMin = 0.0))
		float MaxUltimateDuration;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Time")
		float CurrentUltimateDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Time")
		float UltimateFreq;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
		float UltimateWalkSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Abilities")
		float NormalWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
		float UltimatePlayRate;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Abilities")
		float PlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities")
		float UltimateShootFreq;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
		FName FPSCameraSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		FName MeleeSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Levels")
		FName MainMenuMapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
		TArray<FName> DoorKeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon");
	TSubclassOf<ARP_Weapon> InitialWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ERP_CharacterType CharacterType;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon");
	ARP_Weapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* MeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* UltimateMontage;

	UAnimInstance* MyAnimInstance;

	ARP_GameMode* GameModeReference;

	URP_GameInstance* GameInstaceReference;

	FTimerHandle TimerHandle_Ultimate;

	FTimerHandle TimerHandle_AutomaticShoot;

	FTimerHandle TimerHandle_BeginUltimateBehavior;

public:

	UPROPERTY(BlueprintAssignable)
	FOnUltimateUpdateSignature OnUltimateUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnUltimateStatusSignature OnUltimateStatusDelegate;

public:
	// Sets default values for this character's properties
	ARP_CharacterPlayer();

	virtual FVector GetPawnViewLocation() const override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeReferences();

	void MoveForward(float value);

	void MoveRight(float value);

	virtual void Jump() override;

	virtual void StopJumping() override;

	void StartWeaponAction();

	void StopWeaponAction();

	void CreateInitialWeapon();
	
	UFUNCTION(BlueprintCallable)
	void StartMelee();

	void StopMelee();

	void StartUltimate();

	void GoToMainMenu();

	UFUNCTION()
	void MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHealthChange(URP_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerPitchInput(float value) override;

	void AddKey(FName NewKey);

	bool TryAddHealth(float HealthToAdd);
	
	bool HasKey(FName KeyTag);

	void SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState);

	void SetMeleeState(bool NewState);

	UFUNCTION(BlueprintCallable)
	void SetComboEnable(bool NewState);

	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	bool HasToDestroy() { return bHasToDestroy; };

	UFUNCTION(BlueprintCallable)
	void GainUltimateXP(float XPGained);

	void UpdateUltimateDuration(float Value);

	void UpdateUltimateDurationWithTimer();

	void BeginUltimateBehavior();

	UFUNCTION(BlueprintCallable)
	ERP_CharacterType GetCharacterType() { return CharacterType; };

	URP_HealthComponent* GetHealthComponent() { return HealthComponent; };

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GainUltimateXP(float XPGained);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StartUltimate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StopUltimate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_UpdateUltimateDuration(float Value);
	
};

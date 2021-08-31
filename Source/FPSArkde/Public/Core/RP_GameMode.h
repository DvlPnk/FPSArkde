// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enemy/RP_Enemy.h"
#include "GameFramework/GameModeBase.h"
#include "RP_GameMode.generated.h"

class USoundCue;
class ARP_CharacterPlayer;
class ARP_SpectatingCamera;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyAddedSignature, FName, KeyTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStateChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlertModeChangeSignature, bool, bIsAlert);

UCLASS()
class FPSARKDE_API ARP_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Level")
	bool bIsAlertMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spectating Camera")
	float SpectatingBlendTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow")
	FName MainManuMapName;
	
	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	ARP_SpectatingCamera* VictoryCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	ARP_SpectatingCamera* GameOverCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
	USoundCue* VictoryMusic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
	USoundCue* GameOverMusic;
	
	FTimerHandle TimerHandle_BackToMainMenu;

	UPROPERTY(BlueprintReadOnly, Category = "Level")
	TArray<ARP_Enemy*> LevelEnemies;

public:

	ARP_GameMode();
	
	UPROPERTY(BlueprintAssignable)
	FOnKeyAddedSignature OnKeyAddedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnGameStateChange OnVictoryDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnGameStateChange OnGameOverDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAlertModeChangeSignature OnAlertModeChangeDelegate;
	 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetupSpectatingCameras();

	void MoveCameraSpectatingPoint(ARP_CharacterPlayer* Character, ARP_SpectatingCamera* SpectatingCamera);

	void PlayMusic(USoundCue* MusicCue);
	
public:

	UFUNCTION()
	void AddKeyToCharacter(ARP_CharacterPlayer* KeyOwner, FName KeyTag);
	
	UFUNCTION()
	void Victory(ARP_CharacterPlayer* Character);

	UFUNCTION()
	void GameOver(ARP_CharacterPlayer* Character);

	void BackToMainMenu();

	void CheckAlertMode();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Victory(ARP_CharacterPlayer* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver(ARP_CharacterPlayer* Character); 
	
};

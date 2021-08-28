// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RP_GameMode.generated.h"

class ARP_CharacterPlayer;
class ARP_SpectatingCamera;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyAddedSignature, FName, KeyTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStateChange);

UCLASS()
class FPSARKDE_API ARP_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spectating Camera")
	float SpectatingBlendTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow")
	FName MainManuMapName;
	
	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	ARP_SpectatingCamera* VictoryCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	ARP_SpectatingCamera* GameOverCamera;

	FTimerHandle TimerHandle_BackToMainMenu;

public:

	ARP_GameMode();
	
	UPROPERTY(BlueprintAssignable)
	FOnKeyAddedSignature OnKeyAddedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnGameStateChange OnVictoryDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnGameStateChange OnGameOverDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetupSpectatingCameras();

	void MoveCameraSpectatingPoint(ARP_CharacterPlayer* Character, ARP_SpectatingCamera* SpectatingCamera);

	
	
public:

	UFUNCTION()
	void AddKeyToCharacter(ARP_CharacterPlayer* KeyOwner, FName KeyTag);
	
	UFUNCTION()
	void Victory(ARP_CharacterPlayer* Character);

	UFUNCTION()
	void GameOver(ARP_CharacterPlayer* Character);

	void BackToMainMenu();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Victory(ARP_CharacterPlayer* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver(ARP_CharacterPlayer* Character); 
	
};

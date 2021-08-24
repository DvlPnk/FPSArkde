// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RP_GameMode.generated.h"

class ARP_CharacterPlayer;
class ARP_SpectatingCamera;
/**
 * 
 */
UCLASS()
class FPSARKDE_API ARP_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spectating Camera")
	float SpectatingBlendTime;
	
	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	ARP_SpectatingCamera* VictoryCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	ARP_SpectatingCamera* GameOverCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetupSpectatingCameras();

	void MoveCameraSpectatingPoint(ARP_CharacterPlayer* Character, ARP_SpectatingCamera* SpectatingCamera);

	
	
public:

	UFUNCTION()
	void Victory(ARP_CharacterPlayer* Character);

	UFUNCTION()
	void GameOver(ARP_CharacterPlayer* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Victory(ARP_CharacterPlayer* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver(ARP_CharacterPlayer* Character); 
	
};

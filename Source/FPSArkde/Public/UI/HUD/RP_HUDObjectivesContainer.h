// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RP_HUDObjectivesContainer.generated.h"

class URP_HUDObjective;
class ARP_GameMode;
/**
 * 
 */
UCLASS()
class FPSARKDE_API URP_HUDObjectivesContainer : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	ARP_GameMode* GameModeReference;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	TArray<URP_HUDObjective*> Objectives;


public:

	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION(BlueprintCallable)
	void InitializeObjectives(TArray<URP_HUDObjective*> NewObjectives);
	
	UFUNCTION()
	void UpdateObjectives(FName KeyTag);
	
};

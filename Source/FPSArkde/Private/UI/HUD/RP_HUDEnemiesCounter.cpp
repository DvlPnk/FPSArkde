// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/RP_HUDEnemiesCounter.h"

#include "Core/RP_GameInstance.h"

void URP_HUDEnemiesCounter::InitializeWidget()
{
	GameInstanceReference = Cast<URP_GameInstance>(GetWorld()->GetGameInstance());
	if(IsValid(GameInstanceReference))
	{
		GameInstanceReference->OnEnemyKilledDelegate.AddDynamic(this, &URP_HUDEnemiesCounter::UpdateCounter);
		UpdateCounter(GameInstanceReference->GetEnemiesDefeatedCounter());
	}
}

void URP_HUDEnemiesCounter::UpdateCounter(int EnemyDefeatedCounter)
{
	EnemiesDefeated = EnemyDefeatedCounter;
}

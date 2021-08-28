// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/RP_HUDUltimate.h"

#include "RP_CharacterPlayer.h"
#include "Kismet/GameplayStatics.h"

void URP_HUDUltimate::InitializeWidget()
{
	UltimateColor = UltimateDisableColor;
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		ARP_CharacterPlayer* PlayerCharacter = Cast<ARP_CharacterPlayer>(PlayerPawn);
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->OnUltimateUpdateDelegate.AddDynamic(this, &URP_HUDUltimate::UpdateUltimateValue);
			PlayerCharacter->OnUltimateStatusDelegate.AddDynamic(this, &URP_HUDUltimate::UpdateUltimateStatus);
		}
	}
}

void URP_HUDUltimate::UpdateUltimateValue(float CurrentUltimateXP, float MaxUltimateXP)
{
	UltimatePercent = CurrentUltimateXP / MaxUltimateXP;
}

void URP_HUDUltimate::UpdateUltimateStatus(bool bIsAvailable)
{
	UltimateColor = bIsAvailable ? UltimateEnableColor : UltimateDisableColor;
}

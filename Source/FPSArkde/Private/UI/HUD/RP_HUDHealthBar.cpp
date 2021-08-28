// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/RP_HUDHealthBar.h"

#include "RP_CharacterPlayer.h"
#include "Components/RP_HealthComponent.h"
#include "Kismet/GameplayStatics.h"

void URP_HUDHealthBar::InitializeWidget()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(IsValid(PlayerPawn))
	{
		ARP_CharacterPlayer* PlayerCharacter = Cast<ARP_CharacterPlayer>(PlayerPawn);
		if(IsValid(PlayerCharacter))
		{
			URP_HealthComponent* PlayerHealthComponent = PlayerCharacter->GetHealthComponent();
			if(IsValid(PlayerHealthComponent))
			{
				PlayerHealthComponent->OnHealthUpdateDelegate.AddDynamic(this, &URP_HUDHealthBar::UpdateHealth);
			}
		}
	}
}

void URP_HUDHealthBar::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	HealthPercent = CurrentHealth / MaxHealth;
	HealthColor = FMath::Lerp(EmptyHealthColor, FullHealthColor, HealthPercent);
}

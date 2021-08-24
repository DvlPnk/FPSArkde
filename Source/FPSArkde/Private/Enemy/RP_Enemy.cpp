// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RP_Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Components/RP_HealthComponent.h"
#include "Core/RP_GameInstance.h"
#include "Enemy/Controller/RP_AIController.h"
#include "Weapons/RP_Rifle.h"
#include "Items/RP_Items.h"
#include "Perception/AISense_Damage.h"
#include "Core/RP_GameInstance.h"

ARP_Enemy::ARP_Enemy()
{
	DirectionIndex = 1;
	bLoopPath = false;
	WaitingTimeOnPathPoint = 1.0f;
	XPValue = 20.0f;
	LootProbability = 100.0f;
}

void ARP_Enemy::BeginPlay()
{
	Super::BeginPlay();

	MyAIController = Cast<ARP_AIController>(GetController());

	HealthComponent->OnDeadDelegate.AddDynamic(this, &ARP_Enemy::GiveXP);
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ARP_Enemy::HealthChange);
}

void ARP_Enemy::GiveXP(AActor* DamageCauser)
{
	ARP_CharacterPlayer* PossiblePlayer = Cast<ARP_CharacterPlayer>(DamageCauser);

	if(IsValid(PossiblePlayer) && PossiblePlayer->GetCharacterType() == ERP_CharacterType::CharacterType_Player)
	{
		PossiblePlayer->GainUltimateXP(XPValue);
		TrySpawnLoot();
	}

	ARP_Rifle* PossibleRifle = Cast<ARP_Rifle>(DamageCauser);

	if(IsValid(PossibleRifle))
	{
		ARP_CharacterPlayer* RifleOwner = Cast<ARP_CharacterPlayer>(PossibleRifle->GetOwner());
		if(IsValid(RifleOwner) && RifleOwner->GetCharacterType() == ERP_CharacterType::CharacterType_Player)
		{
			RifleOwner->GainUltimateXP(XPValue);
			TrySpawnLoot();
		}
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	BP_GiveXP(DamageCauser);
}

void ARP_Enemy::HealthChange(URP_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(!IsValid(MyAIController))
	{
		return;
	}
	
	if(CurrentHealthComponent->IsDead())
	{
		MyAIController->UnPossess();

		if(IsValid(GameInstaceReference))
		{
			GameInstaceReference->AddEnemieDefeatedToCounter();
		}
	}
	else
	{
		ARP_Rifle* RifleCauser = Cast<ARP_Rifle>(DamageCauser);
		if(IsValid(RifleCauser))
		{
			AActor* RifleOwner = RifleCauser->GetOwner();
			MyAIController->SetReceiveDamage(true);
			UAISense_Damage::ReportDamageEvent(GetWorld(), this, RifleOwner, Damage, RifleOwner->GetActorLocation(), FVector::ZeroVector);
		}
	}
}

bool ARP_Enemy::TrySpawnLoot()
{
	if(!IsValid(LootItemClass))
	{
		return false;
	}
	
	float SelectedProbability = FMath::RandRange(0.0f, 100.0f);
	if(SelectedProbability<=LootProbability)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ARP_Items>(LootItemClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
	}

	return true;
}

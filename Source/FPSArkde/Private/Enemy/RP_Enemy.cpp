// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RP_Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Components/RP_HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/RP_GameInstance.h"
#include "Enemy/Controller/RP_AIController.h"
#include "Weapons/RP_Rifle.h"
#include "Items/RP_Items.h"
#include "Perception/AISense_Damage.h"
#include "Core/RP_GameInstance.h"
#include "Core/RP_GameMode.h"
#include "UI/Enemies/RP_EnemyHealthBar.h"

ARP_Enemy::ARP_Enemy()
{
	DirectionIndex = 1;
	bLoopPath = false;
	WaitingTimeOnPathPoint = 1.0f;
	XPValue = 20.0f;
	LootProbability = 100.0f;

	WidgetHealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetHealthBarComponent"));
	WidgetHealthBarComponent->SetupAttachment(RootComponent);
	
}

void ARP_Enemy::BeginPlay()
{
	Super::BeginPlay();

	MyAIController = Cast<ARP_AIController>(GetController());

	HealthComponent->OnDeadDelegate.AddDynamic(this, &ARP_Enemy::GiveXP);
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ARP_Enemy::HealthChange);

	UUserWidget* WidgetObject = WidgetHealthBarComponent->GetUserWidgetObject();
	
	if(IsValid(WidgetObject))
	{
		EnemyHealthBar = Cast<URP_EnemyHealthBar>(WidgetObject);

		if(IsValid(EnemyHealthBar))
		{
			HealthComponent->OnHealthUpdateDelegate.AddDynamic(EnemyHealthBar, &URP_EnemyHealthBar::UpdateHealth);
			HideHealthBar();
		}
	}
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

	if(bIsShowingHealthBar)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HideHealthBar);
	}else
	{
		ShowHealthBar();
	}
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_HideHealthBar, this, &ARP_Enemy::HideHealthBar, 1.0f, false);
	
	if(CurrentHealthComponent->IsDead())
	{
		MyAIController->DeactivateAIPerception();
		MyAIController->UnPossess();

		if(IsValid(GameInstaceReference))
		{
			GameInstaceReference->AddEnemieDefeatedToCounter();
		}

		SetIsAlert(false);
		
		HideHealthBar();
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

void ARP_Enemy::ShowHealthBar()
{
	bIsShowingHealthBar = true;
	EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
}

void ARP_Enemy::HideHealthBar()
{
	bIsShowingHealthBar = false;
	EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
}

void ARP_Enemy::SetIsAlert(bool bValue)
{
	bIsAlert = bValue;
	if(IsValid(GameModeReference))
	{
		GameModeReference->CheckAlertMode();
	}
}

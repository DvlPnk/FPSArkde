// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RP_Bot.h"

#include "DrawDebugHelpers.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "RP_CharacterPlayer.h"
#include "Components/RP_HealthComponent.h"
#include "Components/SphereComponent.h"
#include "Core/RP_GameInstance.h"
#include "Enemy/RP_BotSpawner.h"
#include "Enemy/RP_Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Weapons/RP_Rifle.h"
#include "Items/RP_Items.h"
#include "Enemy/RP_BotSpawner.h"
#include "Core/RP_GameInstance.h"

class UNavigationPath;
// Sets default values
ARP_Bot::ARP_Bot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotMeshComponent"));
	BotMeshComponent->SetCanEverAffectNavigation(false);
	BotMeshComponent->SetSimulatePhysics(true);
	RootComponent = BotMeshComponent;

	HealthComponent = CreateDefaultSubobject<URP_HealthComponent>(TEXT("HealthComponent"));

	SelfDestructionDetectorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SelfDestructionDetector"));
	SelfDestructionDetectorComponent->SetupAttachment(RootComponent);
	SelfDestructionDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SelfDestructionDetectorComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SelfDestructionDetectorComponent->SetSphereRadius(150.0f);
	
	MinDistanceToTarget = 200.0f;
	ForceMagnitude = 1000.0f;

	ExplosionDamage = 100.0f;
	ExplosionRadius = 100.0f;

	XPValue = 10.0f;
	LootProbability = 100.0f;
}

// Called when the game starts or when spawned
void ARP_Bot::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(IsValid(PlayerPawn))
	{
		PlayerCharacter = Cast<ARP_CharacterPlayer>(PlayerPawn);
	}

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ARP_Bot::TakingDamage);
	HealthComponent->OnDeadDelegate.AddDynamic(this, &ARP_Bot::GiveXP);

	SelfDestructionDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &ARP_Bot::StartCountdown);

	BotMaterial = BotMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BotMeshComponent->GetMaterial(0));
	
	NextPathPoint = GetNextPathPoint();

	GameInstanceReference = Cast<URP_GameInstance>(GetWorld()->GetGameInstance());
	
}

FVector ARP_Bot::GetNextPathPoint()
{
	if(!IsValid(PlayerCharacter))
	{
		return GetActorLocation();
	}

	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), PlayerCharacter);
	if(NavigationPath->PathPoints.Num() > 1)
	{
		return NavigationPath->PathPoints[1];
	}

	return GetActorLocation();

}

void ARP_Bot::TakingDamage(URP_HealthComponent* CurrrentHealthComponent, AActor* DamagedActor, float Damage,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(IsValid(BotMaterial))
	{
		BotMaterial->SetScalarParameterValue("Pulse", GetWorld()->TimeSeconds);
	}

	if(CurrrentHealthComponent->IsDead())
	{
		if(IsValid(DamageCauser))
		{
			ARP_Rifle* Rifle = Cast<ARP_Rifle>(DamageCauser);
			if (IsValid(Rifle))
			{
				ARP_CharacterPlayer* RifleOwner = Cast<ARP_CharacterPlayer>(Rifle->GetOwner());
				if(IsValid(RifleOwner) && RifleOwner->GetCharacterType() == ERP_CharacterType::CharacterType_Player)
				{
					TrySpawnLoot();
					
					if (IsValid(GameInstanceReference))
					{
						GameInstanceReference->AddEnemieDefeatedToCounter();
					}
				}
			}
		}

		
		
		SelfDestruction();
	}
	
}

void ARP_Bot::SelfDestruction()
{
	if(bIsExploded)
	{
		return;
	}

	bIsExploded = true;

	if (IsValid(ExplosionEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}
	
	TArray<AActor*> IgnoredActors;

	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), 
		ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 20, FColor::Yellow, true, 5.0f, 0.0f, 2.0f);
	}

	if(IsValid(MySpawner))
	{
		MySpawner->NotifyBotDead();
	}
	
	Destroy();
}

void ARP_Bot::StartCountdown(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bIsStartingCountdown)
	{
		return;
	}

	if(OtherActor == PlayerCharacter)
	{
		bIsStartingCountdown = true;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ARP_Bot::SelfDamage, 0.5f, true);
	}
	
}

void ARP_Bot::SelfDamage()
{
	UGameplayStatics::ApplyDamage(this, 20.0f, GetInstigatorController(), nullptr, nullptr);
}

void ARP_Bot::GiveXP(AActor* DamageCauser)
{
	ARP_CharacterPlayer* PossiblePlayer = Cast<ARP_CharacterPlayer>(DamageCauser);

	if (IsValid(PossiblePlayer) && PossiblePlayer->GetCharacterType() == ERP_CharacterType::CharacterType_Player)
	{
		PossiblePlayer->GainUltimateXP(XPValue);
	}

	ARP_Rifle* PossibleRifle = Cast<ARP_Rifle>(DamageCauser);

	if (IsValid(PossibleRifle))
	{
		ARP_CharacterPlayer* RifleOwner = Cast<ARP_CharacterPlayer>(PossibleRifle->GetOwner());
		if (IsValid(RifleOwner) && RifleOwner->GetCharacterType() == ERP_CharacterType::CharacterType_Player)
		{
			RifleOwner->GainUltimateXP(XPValue);
		}
	}
	BP_GiveXP(DamageCauser);
}

bool ARP_Bot::TrySpawnLoot()
{
	if (!IsValid(LootItemClass))
	{
		return false;
	}

	float SelectedProbability = FMath::RandRange(0.0f, 100.0f);
	if (SelectedProbability <= LootProbability)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ARP_Items>(LootItemClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
	}

	return true;
}

// Called every frame
void ARP_Bot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
	
	if(DistanceToTarget <= MinDistanceToTarget)
	{
		NextPathPoint = GetNextPathPoint();
	}
	else
	{
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();
		ForceDirection *= ForceMagnitude;
		
		BotMeshComponent->AddForce(ForceDirection, NAME_None, true);
	}

	if(bDebug)
	{
		DrawDebugSphere(GetWorld(), NextPathPoint, 30.0f, 15, FColor::Purple, false, 0.0f, 0.0f, 1.0f);
	}
}


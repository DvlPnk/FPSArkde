// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/RP_HUDObjectivesContainer.h"

#include "Core/RP_GameMode.h"
#include "UI/HUD/RP_HUDObjective.h"

void URP_HUDObjectivesContainer::InitializeWidget()
{
	GameModeReference = Cast<ARP_GameMode>(GetWorld()->GetAuthGameMode());

	if(IsValid(GameModeReference))
	{
		GameModeReference->OnKeyAddedDelegate.AddDynamic(this, &URP_HUDObjectivesContainer::UpdateObjectives);
	}
}

void URP_HUDObjectivesContainer::InitializeObjectives(TArray<URP_HUDObjective*> NewObjectives)
{
	for(URP_HUDObjective* NewObjective : NewObjectives)
	{
		Objectives.AddUnique(NewObjective);
	}
}

void URP_HUDObjectivesContainer::UpdateObjectives(FName KeyTag)
{

	int ObjectiveIndex = -1;
	
	if(KeyTag == "Key_A")
	{
		ObjectiveIndex = 0;
	}
	else if(KeyTag == "Key_B")
	{
		ObjectiveIndex = 1;
	}

	if(Objectives.IsValidIndex(ObjectiveIndex))
	{
		URP_HUDObjective* CurrentObjetive = Objectives[ObjectiveIndex];
		if(IsValid(CurrentObjetive))
		{
			CurrentObjetive->ObjectiveCompleted();
		}
	}
	
}

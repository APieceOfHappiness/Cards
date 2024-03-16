// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <vector>

#include "WarriorCardTemplate.h"
#include "ChangeCardTemplate.h"
#include "Card.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RealCardsSpawner.generated.h"

UCLASS()
class GODGAMESAGAIN_API ARealCardsSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARealCardsSpawner();

protected:
	std::vector<std::vector<FVector>> WarriorActorsCoords;
	std::vector<std::vector<FVector>> ChangeActorsCoords;

	std::vector<std::vector<AWarriorCardTemplate*>> WarriorActors;
	std::vector<std::vector<AChangeCardTemplate*>> ChangeActors;

	int DelayBetweenCards = 3;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnCards(const std::vector<std::vector<Gen::Warrior*>>& Warriors, 
					const std::vector<std::vector<Gen::CardOfChange*>>& CardsOfChange);
	void SwapCards(int PlayerNumber, int BoardIdx1, int BoardIdx2, bool IsWarrior);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextRenderComponent.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include "WarriorCardTemplate.h"
#include "ChangeCardTemplate.h"
#include "RealCardsSpawner.h"

#include "Engine.h"
#include "Card.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GamePlay.generated.h"

class GameStates {
public:
	static const int STARTED = 0;
	static const int WAITING_FOR_PLAYERS_ANSWER = 1;
	static const int PLAYER_ANSWERED = 2;
	static const int FINISHED = 3;
};

UCLASS()
class GODGAMESAGAIN_API AGamePlay : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGamePlay();
	int NumCards = 6;

	Gen::WarriorDeck WDeck;
	Gen::ChangeDeck CDeck;

	std::vector<std::vector<Gen::Warrior*>> WarriorBoard;
	std::vector<std::vector<Gen::CardOfChange*>> ChangeBoard;

	std::vector<std::vector<int>> Generation;
	std::vector<std::vector<int>> Dataset;
	std::vector<int> BestSolution;

	std::vector<float> FitnessHistory;

	ARealCardsSpawner* CardsSpawner;

	int GenerationSize = 15;
	int DatasetSize = 1000;
	int NumEpoch = 30;

	float MutationProb = 0.1f;

	int GameState = GameStates::STARTED;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PlayRound();
	void Crossover(float CrossProb);
	void Mutate(float MutationProb);
	void Selection(const std::vector<float>& Fitnesses, int NumComp);
	float Train(int NumEpoch);

	std::vector<std::vector<int>> CreateDataset(int Size);
	std::pair<int, int> GetFitness(const std::vector<int>& player_1_deck, const std::vector<int>& player_2_deck);
	std::vector<float>& GetFitnessHistory();

	void SwapCards(int PlayerNumber, int BoardIdx1, int BoardIdx2, bool IsWarrior);
	void Fighting();

	void SetGameState(int GameState);
	int GetGameState();

	std::vector<std::vector<float>> GetParameters();
	void SetParameters(const std::vector<std::vector<float>>& HyperParams);

	void GanerateCardsToTheBoard();
	float Train(int NumEpoch, int DatasetSize, int GenerationSize);
	void DeleteCardsFromTheBoard();
};

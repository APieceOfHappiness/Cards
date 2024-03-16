// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay.h"
#include "GameFramework/Actor.h"
#include "Optimizer.generated.h"


class GODGAMESAGAIN_API AParametersKeeper {
	std::vector<std::vector<float>> Params;

public:
	AParametersKeeper() = default;
	//AParametersKeeper(const std::vector<std::vector<float>>& Params);
	void SetParams(const std::vector<std::vector<float>>& Params);
	std::vector<std::vector<float>> GenerateNewParams(float Prob, int MinElements);
};


UCLASS()
class GODGAMESAGAIN_API AOptimizer : public AActor
{
	GENERATED_BODY()
	
private:
	AGamePlay* GamePlay;
	AParametersKeeper ParametersKeeper;
	std::vector<std::vector<std::vector<float>>> Dataset;

	int EpochCnt = 3;
	int ParametersCnt = 10;
	int NumIters = 50;

public:	
	// Sets default values for this actor's properties
	AOptimizer();

	void SetGamePlay(AGamePlay* GamePlay);
	void FindOptimalParameters();
	void CreateDataset(int DatasetSize);
	void Selection(int NumPlayers, const std::vector<float>& fitnesses);
	void Mutation(float prob);
	void Crossover(float prob);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Optimizer.h"

// Sets default values
AOptimizer::AOptimizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOptimizer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOptimizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOptimizer::SetGamePlay(AGamePlay* GamePlay) {
	this->GamePlay = GamePlay;
	this->ParametersKeeper = AParametersKeeper();
	this->ParametersKeeper.SetParams(GamePlay->GetParameters());
}

void AOptimizer::Selection(int NumPlayers, const std::vector<float>& fitnesses) {
	std::vector<std::vector<std::vector<float>>> NewGeneration;
	int MaxFitnessIdx = -1;
	int RandomIdx = -1;
	for (int i = 0; i < this->ParametersCnt; ++i) {
		MaxFitnessIdx = FMath::RandRange(0, this->ParametersCnt - 1);
		for (int j = 0; j < NumPlayers - 1; ++j) {
			int RandomIdx = FMath::RandRange(0, this->ParametersCnt - 1);
			if (this->Dataset[RandomIdx] > this->Dataset[MaxFitnessIdx]) {
				MaxFitnessIdx = RandomIdx;
			}
		}
		NewGeneration.push_back(this->Dataset[MaxFitnessIdx]);
	}
	std::copy(NewGeneration.begin(), NewGeneration.end(), this->Dataset.begin());
}

void AOptimizer::Mutation(float prob)
{
	for (int i = 0; i < this->ParametersCnt; ++i) {
		int RandomParameterIdx = FMath::RandRange(0, this->Dataset[i].size() - 1);
		int ParSize = this->Dataset[i][RandomParameterIdx].size();
		//if (ParSize) {
		//	if (FMath::FRandRange(0, 1) > 0.5) {
		//		this->Dataset[i][RandomParameterIdx][FMath::RandRange(0, ParSize - 1)].push_back();
		//	}
		//}
	}
}

void AOptimizer::FindOptimalParameters() {
	if (!this->GamePlay) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GameMode is missed")));
		return;
	}

	float RandomFitness = 0;
	float SmartFitness = 0;

	float MeanR2 = 0;
	float MeanDiff = 0;
	std::vector<float> Diffs;

	this->CreateDataset(ParametersCnt);
	for (int Epoch = 0; Epoch < this->EpochCnt; ++Epoch) {
		MeanR2 = 0;
		MeanDiff = 0;
		Diffs.clear();
		for (int ParametersIdx = 0; ParametersIdx < this->ParametersCnt; ++ParametersIdx) {
			this->GamePlay->SetParameters(this->Dataset[ParametersIdx]);
			RandomFitness = 0;
			SmartFitness = 0;

			for (int i = 0; i < this->NumIters; ++i) {
				this->GamePlay->GanerateCardsToTheBoard();
				this->GamePlay->Train(10, 200, 100);
				this->GamePlay->DeleteCardsFromTheBoard();
				RandomFitness += this->GamePlay->GetFitnessHistory().front();
				SmartFitness += this->GamePlay->GetFitnessHistory().back();
			}
			MeanDiff += (SmartFitness - RandomFitness) / this->NumIters;
			MeanR2 += 1 - RandomFitness / SmartFitness;
			Diffs.push_back(MeanDiff);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Random: %f\nSmart: %f\nR2: %f"), RandomFitness / NumIters, SmartFitness / NumIters, 1 - RandomFitness / SmartFitness));
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MeanR2: %f"), MeanR2 / ParametersCnt));
		UE_LOG(LogTemp, Warning, TEXT("MeanR2: %f, MeanDiff: %f"), MeanR2 / this->ParametersCnt, MeanDiff / this->ParametersCnt);

		this->Selection(3, Diffs);
	}
}

void AOptimizer::CreateDataset(int DatasetSize)
{
	this->Dataset.resize(DatasetSize, std::vector<std::vector<float>>());
	for (int i = 0; i < DatasetSize; ++i) {
		this->Dataset[i] = this->ParametersKeeper.GenerateNewParams(0.2, 3);
	}
}



void AParametersKeeper::SetParams(const std::vector<std::vector<float>>& Params)
{
	this->Params.resize(Params.size(), std::vector<float>());
	for (int i = 0; i < Params.size(); ++i) {
		for (int j = 0; j < Params[i].size(); ++j) {
			this->Params[i].push_back(Params[i][j]);
		}
	}
}

std::vector<std::vector<float>> AParametersKeeper::GenerateNewParams(float Prob, int MinElements)
{
	std::vector<std::vector<float>> NewParams(this->Params.size(), std::vector<float>());
	for (int i = 0; i < this->Params.size(); ++i) {
		for (int j = 0; j < this->Params[i].size(); ++j) {
			if (FMath::FRandRange(0, 1) <= Prob) {
				NewParams[i].push_back(this->Params[i][j]);
			}
		}
		if ((NewParams[i].size() <= MinElements) && (this->Params[i].size() > MinElements)) {
			NewParams[i].clear();
			NewParams.pop_back();
		}
	}

	return NewParams;
}

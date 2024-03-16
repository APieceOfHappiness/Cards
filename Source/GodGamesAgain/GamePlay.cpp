// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay.h"
#include "Logging/LogMacros.h"

// Sets default values
AGamePlay::AGamePlay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->WDeck = Gen::WarriorDeck();
	this->CDeck = Gen::ChangeDeck();
	if (!GetWorld()) {
		UE_LOG(LogTemp, Warning, TEXT("WHAT"));
	}
}

// Called when the game starts or when spawned
void AGamePlay::BeginPlay()
{
	Super::BeginPlay();
	this->CardsSpawner = GetWorld()->SpawnActor<ARealCardsSpawner>(FVector::ZeroVector, FRotator::ZeroRotator);

	//float RandomFitness = 0;
	//float SmartFitness = 0;
	//int NumIters = 50;
	//for (int i = 0; i < NumIters; ++i) {
	//	this->GanerateCardsToTheBoard();
	//	this->Train(10, 500, 200);
	//	this->DeleteCardsFromTheBoard();
	//	RandomFitness += this->GetFitnessHistory().front();
	//	SmartFitness += this->GetFitnessHistory().back();
	//}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Random: %f\nSmart: %f\nR2: %f"), RandomFitness / NumIters, SmartFitness / NumIters, 1 - RandomFitness / SmartFitness));

	// Basicaly I need this:
	this->PlayRound();
}

std::vector<float>& AGamePlay::GetFitnessHistory()
{
	return this->FitnessHistory;
}

void AGamePlay::SetGameState(int GameState)
{
	this->GameState = GameState;
}

void AGamePlay::Fighting()
{
	std::vector<int> classic_order;
	for (int i = 0; i < this->NumCards; ++i) {
		classic_order.push_back(i);
	}
	std::pair<int, int> res = this->GetFitness(classic_order, this->BestSolution);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Me: %d\nOpponent: %d"), res.first, res.second));
	this->GameState = GameStates::FINISHED;
}

// Called every frame
void AGamePlay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->GameState == GameStates::PLAYER_ANSWERED) {
		this->Fighting();
		this->DeleteCardsFromTheBoard();
	}
}

void AGamePlay::PlayRound()
{
	this->GameState = GameStates::STARTED;
	this->GanerateCardsToTheBoard();

	this->Train(this->NumEpoch);

	this->CardsSpawner->SpawnCards(this->WarriorBoard, this->ChangeBoard);
	this->GameState = GameStates::WAITING_FOR_PLAYERS_ANSWER;
}


float AGamePlay::Train(int NumEpoch, int DatasetSize, int GenerationSize) 
{
	this->FitnessHistory.clear();

	this->GenerationSize = GenerationSize;
	this->DatasetSize = DatasetSize;
	this->Generation = this->CreateDataset(GenerationSize);
	this->Dataset = this->CreateDataset(DatasetSize);
	std::vector<float> EpochFitnesses;
	float MaxFitness = 0;

	for (int i = 0; i < NumEpoch; ++i) {
		MaxFitness = 0;
		EpochFitnesses.clear();
		float GenerationFitness = 0;
		for (int player_2_idx = 0; player_2_idx < this->GenerationSize; ++player_2_idx) {
			float MeanFitness = 0;
			for (int player_1_idx = 0; player_1_idx < this->DatasetSize; ++player_1_idx) {
				MeanFitness += this->GetFitness(this->Dataset[player_1_idx], this->Generation[player_2_idx]).second;
			}
			MeanFitness /= this->DatasetSize;
			if (MeanFitness > MaxFitness) {
				MaxFitness = MeanFitness;
				this->BestSolution = this->Dataset[player_2_idx];
			}

			EpochFitnesses.push_back(MeanFitness);
			GenerationFitness += EpochFitnesses.back();
		}
		GenerationFitness /= this->GenerationSize;
		this->FitnessHistory.push_back(GenerationFitness);
		//if (MaxFitness < GenerationFitness) {
		//	MaxFitness = GenerationFitness;
		//}
		UE_LOG(LogTemp, Warning, TEXT("Mean fitness: %f"), GenerationFitness);
		this->Mutate(this->MutationProb);
		//this->Crossover();
		this->Selection(EpochFitnesses, 3);
	}
	UE_LOG(LogTemp, Warning, TEXT("***"));
	return MaxFitness;
}

float AGamePlay::Train(int NumEpoch)
{
	return this->Train(NumEpoch, this->DatasetSize, this->GenerationSize);
}

std::vector<std::vector<int>> AGamePlay::CreateDataset(int Size)
{
	std::vector<int> original;
	for (int i = 0; i < this->NumCards; ++i) {
		original.push_back(i);
	}
	std::vector<std::vector<int>> PermDataset;

	for (int i = 0; i < Size; ++i) {
		std::vector<int> shuffled = original;
		std::random_shuffle(shuffled.begin(), shuffled.end());
		PermDataset.push_back(shuffled);
	}
	return PermDataset;
}

std::pair<int, int> AGamePlay::GetFitness(const std::vector<int>& player_1_deck, const std::vector<int>& player_2_deck)
{
	std::vector<Gen::Warrior*> temp;
	int cur_pos = 0;
	for (auto card_id : player_1_deck) {
		int IsPositive = this->ChangeBoard[0][card_id]->IsPositive();
		int PosType = this->ChangeBoard[0][card_id]->GetRelativeTarget();
		if (PosType == Gen::RTarget::SINGLE) {
			temp.push_back(this->WarriorBoard[1 - IsPositive][cur_pos]);
		}
		else if (PosType == Gen::RTarget::DIAG) {
			if (cur_pos > 0) {
				temp.push_back(this->WarriorBoard[1 - IsPositive][cur_pos - 1]);
			}
			if (cur_pos < this->NumCards - 1) {
				temp.push_back(this->WarriorBoard[1 - IsPositive][cur_pos + 1]);
			}
		}
		this->ChangeBoard[0][card_id]->Apply(temp);

		temp.clear();
		cur_pos++;
	}

	cur_pos = 0;
	for (auto card_id : player_2_deck) {
		int IsPositive = this->ChangeBoard[1][card_id]->IsPositive();
		int PosType = this->ChangeBoard[1][card_id]->GetRelativeTarget();
		if (PosType == Gen::RTarget::SINGLE) {
			temp.push_back(this->WarriorBoard[IsPositive][cur_pos]);
		}
		else if (PosType == Gen::RTarget::DIAG) {
			if (cur_pos > 0) {
				temp.push_back(this->WarriorBoard[IsPositive][cur_pos - 1]);
			}
			if (cur_pos < this->NumCards - 1) {
				temp.push_back(this->WarriorBoard[IsPositive][cur_pos + 1]);
			}
		}
		this->ChangeBoard[1][card_id]->Apply(temp);

		temp.clear();
		cur_pos++;
	}

	int player_1_result = 0;
	int player_2_result = 0;
	for (int i = 0; i < this->NumCards; ++i) {
		if (this->WarriorBoard[0][i]->GetState() == Gen::State::ALIVE && this->WarriorBoard[1][i]->GetState() == Gen::State::ALIVE) {
			this->WarriorBoard[0][i]->Attack(this->WarriorBoard[1][i]->GetDamage());
			this->WarriorBoard[1][i]->Attack(this->WarriorBoard[0][i]->GetDamage());
		}

		if (this->WarriorBoard[0][i]->GetState() == Gen::State::ALIVE) {
			player_1_result += 1;
		}
		else {
			player_2_result += 1;
		}
		if (this->WarriorBoard[1][i]->GetState() == Gen::State::ALIVE) {
			player_2_result += 1;
		}
		else {
			player_1_result += 1;
		}
		this->WarriorBoard[0][i]->Recreate();
		this->WarriorBoard[1][i]->Recreate();
	}
	return std::make_pair(player_1_result, player_2_result);
}

void AGamePlay::Crossover(float CrossProb)
{
	//for (int i = 0; i < this->GenerationSize; ++i) {
	//	int idx1 = F
	//}
}

void AGamePlay::SwapCards(int PlayerNumber, int BoardIdx1, int BoardIdx2, bool IsWarrior)
{
	if (IsWarrior) {
		Gen::Warrior* temp = this->WarriorBoard[PlayerNumber - 1][BoardIdx1];
		this->WarriorBoard[PlayerNumber - 1][BoardIdx1] = this->WarriorBoard[PlayerNumber - 1][BoardIdx2];
		this->WarriorBoard[PlayerNumber - 1][BoardIdx2] = temp;
	}
	else {
		Gen::CardOfChange* temp = this->ChangeBoard[PlayerNumber - 1][BoardIdx1];
		this->ChangeBoard[PlayerNumber - 1][BoardIdx1] = this->ChangeBoard[PlayerNumber - 1][BoardIdx2];
		this->ChangeBoard[PlayerNumber - 1][BoardIdx2] = temp;
	}
	this->CardsSpawner->SwapCards(PlayerNumber, BoardIdx1, BoardIdx2, IsWarrior);
}

void AGamePlay::Mutate(float MutateProb)
{
	for (int i = 0; i < this->GenerationSize; ++i) {
		if (FMath::RandRange(0.0f, 1.0f) < MutateProb) {
			int idx1 = FMath::RandRange(0, this->NumCards - 1);
			int idx2 = FMath::RandRange(0, this->NumCards - 1);
			int c = this->Generation[i][idx1];
			this->Generation[i][idx1] = this->Generation[i][idx2];
			this->Generation[i][idx2] = c;
		}
	}
}

void AGamePlay::Selection(const std::vector<float>& Fitnesses, int NumComp)
{
	std::vector<std::vector<int>> NewGeneration;
	for (int i = 0; i < this->GenerationSize; ++i) {
		int BestIndex = FMath::RandRange(0, this->GenerationSize - 1);
		for (int j = 0; j < NumComp - 1; ++j) {
			int NewIdx = FMath::RandRange(0, this->GenerationSize - 1);
			if (Fitnesses[NewIdx] > Fitnesses[BestIndex]) {
				BestIndex = NewIdx;
			}
		}
		NewGeneration.push_back(this->Generation[BestIndex]);
	}
	this->Generation.clear();
	this->Generation = NewGeneration;
}

int AGamePlay::GetGameState() {
	return this->GameState;
}

void AGamePlay::GanerateCardsToTheBoard() {
	this->WarriorBoard.resize(2, std::vector<Gen::Warrior*>(this->NumCards));
	this->ChangeBoard.resize(2, std::vector<Gen::CardOfChange*>(this->NumCards));
	for (int i = 0; i < this->NumCards; ++i) {
		this->WarriorBoard[0][i] = this->WDeck.GenerateCard();
		this->WarriorBoard[1][i] = this->WDeck.GenerateCard();
		this->ChangeBoard[0][i] = this->CDeck.GenerateCard();
		this->ChangeBoard[1][i] = this->CDeck.GenerateCard();
	}
}

std::vector<std::vector<float>> AGamePlay::GetParameters()
{
	std::vector<std::vector<float>> Params = this->CDeck.GetParams();
	for (auto& WarriorParam : this->WDeck.GetParams()) {
		Params.push_back(WarriorParam);
	}
	return Params;
}

void AGamePlay::SetParameters(const std::vector<std::vector<float>>& HyperParams) {
	std::vector<std::vector<float>> ChangeParams(5, std::vector<float>());
	std::vector<std::vector<float>> WarriorParams(2, std::vector<float>());
	UE_LOG(LogTemp, Warning, TEXT("HyperParams: %d"), HyperParams.size());
	for (int i = 0; i < 5; ++i) {
		//ChangeParams.push_back(HyperParams[i]);
		ChangeParams[i].resize(HyperParams[i].size());
		std::copy(HyperParams[i].begin(), HyperParams[i].end(), ChangeParams[i].begin());
	}
	for (int i = 5; i < 7; ++i) {
		//WarriorParams.push_back(HyperParams[i]);
		WarriorParams[i - 5].resize(HyperParams[i].size());
		std::copy(HyperParams[i].begin(), HyperParams[i].end(), WarriorParams[i - 5].begin());
	}
	this->CDeck.SetParams(ChangeParams);
	this->WDeck.SetParams(WarriorParams);
}

void AGamePlay::DeleteCardsFromTheBoard() {
	for (int i = 0; i < this->NumCards; ++i) {
		delete this->WarriorBoard[0][i];
		delete this->WarriorBoard[1][i];
		delete this->ChangeBoard[0][i];
		delete this->ChangeBoard[1][i];
	}
	this->ChangeBoard.clear();
	this->WarriorBoard.clear();
}
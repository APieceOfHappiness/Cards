// Fill out your copyright notice in the Description page of Project Settings.


#include "RealCardsSpawner.h"

// Sets default values
ARealCardsSpawner::ARealCardsSpawner() :
	WarriorActorsCoords{
	{FVector(200.f, 320.f, 20.f), FVector(200.f, 210.f, 20.f), FVector(200.f, 100.f, 20.f),
	 FVector(200.f, -100.f, 20.f), FVector(200.f, -210.f, 20.f), FVector(200.f, -320.f, 20.f)},
	{FVector(0.f, 320.f, 20.f), FVector(0.f, 210.f, 20.f), FVector(0.f, 100.f, 20.f),
	 FVector(0.f, -100.f, 20.f), FVector(0.f, -210.f, 20.f), FVector(0.f, -320.f, 20.f)},
},
	ChangeActorsCoords {
	{FVector(350.f, 320.f, 20.f), FVector(350.f, 210.f, 20.f), FVector(350.f, 100.f, 20.f),
	 FVector(350.f, -100.f, 20.f), FVector(350.f, -210.f, 20.f), FVector(350.f, -320.f, 20.f)},
	{FVector(-150.f, 320.f, 20.f), FVector(-150.f, 210.f, 20.f), FVector(-150.f, 100.f, 20.f),
	 FVector(-150.f, -100.f, 20.f), FVector(-150.f, -210.f, 20.f), FVector(-150.f, -320.f, 20.f)},
}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->WarriorActors.resize(2, std::vector<AWarriorCardTemplate*>());
	this->ChangeActors.resize(2, std::vector<AChangeCardTemplate*>());
}

// Called when the game starts or when spawned
void ARealCardsSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARealCardsSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARealCardsSpawner::SpawnCards(const std::vector<std::vector<Gen::Warrior*>>& Warriors,
								   const std::vector<std::vector<Gen::CardOfChange*>>& CardsOfChange)
{
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < Warriors[i].size(); ++j) {
			FString CardInfo = Warriors[i][j]->GetDescription();
			this->WarriorActors[i].push_back(GetWorld()->SpawnActor<AWarriorCardTemplate>(FVector(100.f, 0.f, (i * Warriors[i].size() + j + 1) + 30), FRotator::ZeroRotator));
			this->WarriorActors[i].back()->ChangeText(CardInfo);
			this->WarriorActors[i].back()->ChangeBoardPosInfo(i + 1, j);
			this->WarriorActors[i].back()->StartMovingTo(this->WarriorActorsCoords[i][j], 2.5f, (i * Warriors[i].size() + j + 1) * this->DelayBetweenCards);
		}
	}
	int WarriorsCnt = Warriors[0].size() * 2;
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < CardsOfChange[i].size(); ++j) {
			FString CardInfo = CardsOfChange[i][j]->GetDescription();
			this->ChangeActors[i].push_back(GetWorld()->SpawnActor<AChangeCardTemplate>(FVector(100.f, 0.f, (WarriorsCnt + i * CardsOfChange[i].size() + j + 1) + 30), FRotator::ZeroRotator));
			this->ChangeActors[i].back()->ChangeText(CardInfo);
			this->ChangeActors[i].back()->ChangeBoardPosInfo(i + 1, j);
			this->ChangeActors[i].back()->StartMovingTo(this->ChangeActorsCoords[i][j], 2.5f, (WarriorsCnt + i * CardsOfChange[i].size() + j + 1) * this->DelayBetweenCards);
			if (i == 0) {
				this->ChangeActors[i].back()->SetMovableByCharacter(true);
			}
		}
	}
}

void ARealCardsSpawner::SwapCards(int PlayerNumber, int BoardIdx1, int BoardIdx2, bool IsWarrior)
{
	if (IsWarrior) {
		AWarriorCardTemplate* temp = this->WarriorActors[PlayerNumber - 1][BoardIdx1];
		this->WarriorActors[PlayerNumber - 1][BoardIdx1] = this->WarriorActors[PlayerNumber - 1][BoardIdx2];
		this->WarriorActors[PlayerNumber - 1][BoardIdx2] = temp;

		this->WarriorActors[PlayerNumber - 1][BoardIdx2]->StartMovingTo(this->WarriorActorsCoords[PlayerNumber - 1][BoardIdx2], 3.f, 0.f);
		this->WarriorActors[PlayerNumber - 1][BoardIdx1]->StartMovingTo(this->WarriorActorsCoords[PlayerNumber - 1][BoardIdx1], 3.f, 0.f);

		this->WarriorActors[PlayerNumber - 1][BoardIdx1]->ChangeBoardPosInfo(PlayerNumber, BoardIdx1);
		this->WarriorActors[PlayerNumber - 1][BoardIdx2]->ChangeBoardPosInfo(PlayerNumber, BoardIdx2);
	}
	else {
		AChangeCardTemplate* temp = this->ChangeActors[PlayerNumber - 1][BoardIdx1];
		this->ChangeActors[PlayerNumber - 1][BoardIdx1] = this->ChangeActors[PlayerNumber - 1][BoardIdx2];
		this->ChangeActors[PlayerNumber - 1][BoardIdx2] = temp;

		this->ChangeActors[PlayerNumber - 1][BoardIdx2]->StartMovingTo(this->ChangeActorsCoords[PlayerNumber - 1][BoardIdx2], 3.f, 0.f);
		this->ChangeActors[PlayerNumber - 1][BoardIdx1]->StartMovingTo(this->ChangeActorsCoords[PlayerNumber - 1][BoardIdx1], 3.f, 0.f);

		this->ChangeActors[PlayerNumber - 1][BoardIdx1]->ChangeBoardPosInfo(PlayerNumber, BoardIdx1);
		this->ChangeActors[PlayerNumber - 1][BoardIdx2]->ChangeBoardPosInfo(PlayerNumber, BoardIdx2);
	}
}


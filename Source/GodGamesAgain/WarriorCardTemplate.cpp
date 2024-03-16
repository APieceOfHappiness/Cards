// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorCardTemplate.h"

AWarriorCardTemplate::AWarriorCardTemplate() {
	this->CubeComponent->SetWorldScale3D(FVector(0.05f, 1.f, 1.5f));
}


void AWarriorCardTemplate::BeginPlay() {
	Super::BeginPlay();
	//FString Text = FString::Printf(TEXT("HP\n|%d|\nDamage\n|%d|"), 2, 3);
	FString Text = FString::Printf(TEXT("Warrior"));
	this->ChangeText(Text);
}


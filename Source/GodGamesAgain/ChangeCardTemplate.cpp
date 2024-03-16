// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeCardTemplate.h"


AChangeCardTemplate::AChangeCardTemplate() {
	this->CubeComponent->SetWorldScale3D(FVector(0.05f, 1.f, 1.f));
}


void AChangeCardTemplate::BeginPlay() {
	Super::BeginPlay();
	//FString Text = FString::Printf(TEXT("Change\n|%d|"), 2);
	FString Text = FString::Printf(TEXT("Change"));
	this->ChangeText(Text);
}


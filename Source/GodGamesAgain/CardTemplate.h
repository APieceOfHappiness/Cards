// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardTemplate.generated.h"

UCLASS()
class GODGAMESAGAIN_API ACardTemplate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACardTemplate();

protected:
	UStaticMeshComponent* CubeComponent;
	UTextRenderComponent* TextRenderComponent;
	
	// ------------ Может ли её двигать Character 
	int MovableByCharacter = 0;

	// ------------ Информация о расположении
	int PlayerNumber;
	int BoardIdx;

	// ------------ Чтобы карта могла передвигаться плавно
	void MoveTo(const FVector& Target, float Speed);
	int TicksRemainedForMoving = 0;
	FVector Target;
	float Speed = 0.5f;
	int Delay = 0;
	// ---------------------------------------------------

	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void StartMovingTo(const FVector& Target, float Speed, int Delay);

	void ChangeText(const FString& FStr);
	void ChangeBoardPosInfo(int PlayerNumber, int BoardIdx);
	void SetMovableByCharacter(int MovableByCharacter);
	int GetMovableByCharacter() const;
	int GetPlayerNumber() const;
	int GetBoardIdx() const;
};




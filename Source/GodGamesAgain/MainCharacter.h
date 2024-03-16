// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GamePlay.h"
#include "Optimizer.h"
#include "ChangeCardTemplate.h"
#include "MainCharacter.generated.h"

UCLASS()
class GODGAMESAGAIN_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

private: 

	AOptimizer* Optimizer;
	AGamePlay* GamePlay;

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Scene")
		USceneComponent* Scene;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Camera")
		UCameraComponent* Camera;

	int ChosenPlayerNumer = -1;
	int ChosenPlayerBoardIdx = -1;

	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnMouseClick();
	void OnEnterClick();
};

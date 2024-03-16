// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	this->Scene = CreateDefaultSubobject<USceneComponent>(TEXT("MainScene"));
	
	RootComponent = this->Scene;
	this->Camera->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->GamePlay = GetWorld()->SpawnActor<AGamePlay>(FVector::ZeroVector, FRotator::ZeroRotator);
	this->Optimizer = GetWorld()->SpawnActor<AOptimizer>(FVector::ZeroVector, FRotator::ZeroRotator);

	//this->Optimizer->SetGamePlay(this->GamePlay);
	//this->Optimizer->FindOptimalParameters();

	this->Scene->SetWorldLocation(FVector(390.f, 0.f, 660.f));
	this->Scene->SetWorldRotation(FQuat(FRotator(-70.f, -180.f, 0.f)));

	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent) {
		PlayerInputComponent->BindAction("MouseClick", IE_Pressed, this, &AMainCharacter::OnMouseClick);
		PlayerInputComponent->BindAction("FinishedThinking", IE_Pressed, this, &AMainCharacter::OnEnterClick);
	}
}

void AMainCharacter::OnMouseClick() {
	FHitResult HitResult;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController && (this->GamePlay->GameState == GameStates::WAITING_FOR_PLAYERS_ANSWER))
	{
		// Получаем информацию о попадании под курсором
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		AActor* ChosenActor = HitResult.GetActor();

		AChangeCardTemplate* ChosenCard = Cast<AChangeCardTemplate>(ChosenActor);
		if (ChosenCard && ChosenCard->GetMovableByCharacter()) {
			if (this->ChosenPlayerNumer == -1) {
				this->ChosenPlayerNumer = ChosenCard->GetPlayerNumber();
				this->ChosenPlayerBoardIdx = ChosenCard->GetBoardIdx();
				FVector PickedUpLocation = ChosenCard->GetActorLocation();
				PickedUpLocation.Z = 50;
				ChosenCard->StartMovingTo(PickedUpLocation, 3.f, 0.f);
			}
			else {
				this->GamePlay->SwapCards(this->ChosenPlayerNumer, this->ChosenPlayerBoardIdx, ChosenCard->GetBoardIdx(), false);
				this->ChosenPlayerNumer = -1;
				this->ChosenPlayerBoardIdx = -1;
			}
			UE_LOG(LogTemp, Warning, TEXT("N: %d IDX: %d"), this->ChosenPlayerNumer, this->ChosenPlayerBoardIdx);
		}
	}
}

void AMainCharacter::OnEnterClick()
{
	this->GamePlay->SetGameState(GameStates::PLAYER_ANSWERED);
}




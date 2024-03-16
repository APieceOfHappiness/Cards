// Fill out your copyright notice in the Description page of Project Settings.


#include "CardTemplate.h"

// Sets default values
ACardTemplate::ACardTemplate()
{
    UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    UMaterial* BasicShapeMaterial = LoadObject<UMaterial>(nullptr, TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
    //UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BasicShapeMaterial, this);

    PrimaryActorTick.bCanEverTick = true;

    this->CubeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    this->CubeComponent->SetStaticMesh(CubeMesh);
    this->CubeComponent->SetMaterial(0, BasicShapeMaterial);

    this->TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
    this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    this->CubeComponent->SetupAttachment(RootComponent);  // Присоединяем CubeComponent к RootComponent
    this->TextRenderComponent->SetupAttachment(RootComponent);  // Присоединяем TextRenderComponent к RootComponent

    // поворот карты
    RootComponent->SetWorldRotation(FQuat(FRotator(90, 0, 0)));

    // Выравнивание текста посередине + цвет + размер
    float TextScale = 25.0f; // Масштаб текста
    FColor NewColor = FColor::Black;
    this->TextRenderComponent->SetWorldSize(TextScale);
    this->TextRenderComponent->SetTextRenderColor(NewColor);
    TextRenderComponent->SetHorizontalAlignment(EHTA_Center);
    TextRenderComponent->SetVerticalAlignment(EVRTA_TextCenter);
    TextRenderComponent->AddLocalOffset(FVector(3.0f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void ACardTemplate::MoveTo(const FVector& Target, float Speed) {
    FVector CurrentLocation = GetActorLocation();
    FVector NewLocation = FMath::VInterpTo(CurrentLocation, Target, GetWorld()->GetDeltaSeconds(), Speed);
    SetActorLocation(NewLocation);
}

// Called every frame
void ACardTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (this->Delay) {
        this->Delay--;
    }
    if (this->TicksRemainedForMoving && !this->Delay) {
        this->MoveTo(this->Target, this->Speed);
        this->TicksRemainedForMoving--;
    }
}

void ACardTemplate::ChangeText(const FString& FStr) {
    this->TextRenderComponent->SetText(FStr);
}

void ACardTemplate::StartMovingTo(const FVector& Target, float Speed, int Delay)
{
    this->TicksRemainedForMoving = 150;
    this->Target = Target;
    this->Speed = Speed;
    this->Delay = Delay;
}

void ACardTemplate::ChangeBoardPosInfo(int PlayerNumber, int BoardIdx)
{
    this->PlayerNumber = PlayerNumber;
    this->BoardIdx = BoardIdx;
}

void ACardTemplate::SetMovableByCharacter(int MovableByCharacter)
{
    this->MovableByCharacter = MovableByCharacter;
}

int ACardTemplate::GetMovableByCharacter() const
{
    return this->MovableByCharacter;
}

int ACardTemplate::GetPlayerNumber() const
{
    return this->PlayerNumber;
}

int ACardTemplate::GetBoardIdx() const
{
    return this->BoardIdx;
}




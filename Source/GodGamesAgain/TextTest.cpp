// Fill out your copyright notice in the Description page of Project Settings.


#include "TextTest.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATextTest::ATextTest()
{
 //   UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
 //   UMaterial* BasicShapeMaterial = LoadObject<UMaterial>(nullptr, TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
 //   //UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BasicShapeMaterial, this);

	//PrimaryActorTick.bCanEverTick = false;

 //   CubeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
 //   CubeComponent->SetStaticMesh(CubeMesh);
 //   CubeComponent->SetMaterial(0, BasicShapeMaterial);

 //   TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
 //   RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

 //   FColor NewColor = FColor::Black;
 //   TextRenderComponent->SetTextRenderColor(NewColor);

 //   CubeComponent->SetupAttachment(RootComponent);  // Присоединяем CubeComponent к RootComponent
 //   TextRenderComponent->SetupAttachment(RootComponent);  // Присоединяем TextRenderComponent к RootComponent


}

// Called when the game starts or when spawned
void ATextTest::BeginPlay()
{
	Super::BeginPlay();

    //if (TextRenderComponent)
    //{
    //    TextRenderComponent->SetText(TEXT("Hello, World!"));
    //}
}

// Called every frame
void ATextTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


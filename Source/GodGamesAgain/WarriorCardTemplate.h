// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardTemplate.h"
#include "WarriorCardTemplate.generated.h"

/**
 * 
 */
UCLASS()
class GODGAMESAGAIN_API AWarriorCardTemplate : public ACardTemplate
{
	GENERATED_BODY()

public:
	AWarriorCardTemplate();
	virtual void BeginPlay() override;
};

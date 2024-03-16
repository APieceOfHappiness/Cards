// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardTemplate.h"
#include "ChangeCardTemplate.generated.h"

/**
 * 
 */
UCLASS()
class GODGAMESAGAIN_API AChangeCardTemplate : public ACardTemplate
{
	GENERATED_BODY()

public:
	AChangeCardTemplate();
	virtual void BeginPlay() override;
};

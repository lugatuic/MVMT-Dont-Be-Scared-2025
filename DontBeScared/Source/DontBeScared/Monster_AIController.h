// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Monster_AIController.generated.h"

/**
 * 
 */
UCLASS()
class DONTBESCARED_API AMonster_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AMonster_AIController(FObjectInitializer const& ObjectInitializer);
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
};

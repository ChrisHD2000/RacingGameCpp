// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AI.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "ControladorAI.generated.h"

/**
 * 
 */
UCLASS()
class CARROBYCHRIS_API AControladorAI : public AAIController
{
	GENERATED_BODY()
public:
		AControladorAI();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		UBehaviorTree* Comportamiento = nullptr;
		UBlackboardData* Data;
		UBlackboardComponent* BBComponent;
		AAI* vehiculoAI;		
		void acelerar();
		UPROPERTY(BlueprintReadWrite)
		float probabilidad = 0.f;
		float random;
protected:
		UPROPERTY(BlueprintReadWrite)
		bool esTutorial = false;
};

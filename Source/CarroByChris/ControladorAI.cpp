   // Fill out your copyright notice in the Description page of Project Settings.


#include "ControladorAI.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "AI.h"
AControladorAI::AControladorAI() {

	
}

void AControladorAI::BeginPlay()
{
	Super::BeginPlay();
	//GetWorldTimerManager().ClearTimer(tiempo);
	vehiculoAI = Cast<AAI>(GetPawn());
}

void AControladorAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (vehiculoAI) {
		FTimerHandle tiempo;
		GetWorldTimerManager().SetTimer(tiempo, this, &AControladorAI::acelerar, 4.f);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController NOT possesing "))
	}
}

void AControladorAI::acelerar()
{
	
		vehiculoAI->MoveForward(1.f);
	if (!esTutorial) {
		random = FMath::RandRange(0.f, 1.f);
		if (random < probabilidad) {
			vehiculoAI->UsePowerUp();
		}	
	}
	
}


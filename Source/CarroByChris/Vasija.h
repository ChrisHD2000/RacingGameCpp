// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vasija.generated.h"
class UStaticMeshComponent;
UCLASS()
class CARROBYCHRIS_API AVasija : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVasija();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UStaticMeshComponent* vasijaMesh;
	UPROPERTY(EditAnywhere)
	float angulo = 1.f;
};

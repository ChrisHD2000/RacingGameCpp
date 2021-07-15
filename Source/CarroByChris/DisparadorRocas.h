// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DisparadorRocas.generated.h"

class UAudioComponent;

UCLASS()
class CARROBYCHRIS_API ADisparadorRocas : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADisparadorRocas();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProyectil> ProjectileClass;
	UPROPERTY(EditAnywhere)
		UAudioComponent* sonido;
	UFUNCTION()
		void Fire();
};

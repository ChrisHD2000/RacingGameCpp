// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CerrarComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CARROBYCHRIS_API UCerrarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCerrarComponent();
	UPROPERTY(EditAnywhere)
	float step = 15.f;
	UPROPERTY(EditAnywhere)
	float distancia = 400.f;
	UPROPERTY(EditAnywhere)
	bool bEmpiezaCerrada = false;
	float maxDist;
	float minDist;
	float delta_Time;
	FVector pos;
	AActor* pared;
	UPROPERTY(EditAnywhere)
	int direccion = -1;
	void moverse();
	void calcularDireccion(int& dir);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

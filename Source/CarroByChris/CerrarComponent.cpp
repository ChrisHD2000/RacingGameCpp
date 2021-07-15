// Fill out your copyright notice in the Description page of Project Settings.


#include "CerrarComponent.h"
#include "GameFramework/Actor.h"
// Sets default values for this component's properties
UCerrarComponent::UCerrarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UCerrarComponent::BeginPlay()
{
	Super::BeginPlay();
	pared = GetOwner();
	pos = pared->GetActorLocation();
	// ...
	maxDist = pared->GetActorLocation().X  - distancia;
	minDist = pared->GetActorLocation().X + distancia;
}


// Called every frame
void UCerrarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	delta_Time = DeltaTime;
	calcularDireccion(direccion);
	
}

void UCerrarComponent::moverse() {
	
	pos.X = pos.X + direccion *step*delta_Time*100.f;
	pared->SetActorLocation(pos);
}

void UCerrarComponent::calcularDireccion(int &dir) {
	if (pared->GetActorLocation().X < maxDist) {
		dir = 1;
	}
	if (pared->GetActorLocation().X > minDist) {
		dir = -1;
	}
	moverse();
}
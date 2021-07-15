// Fill out your copyright notice in the Description page of Project Settings.


#include "Vasija.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AVasija::AVasija()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	vasijaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla Vasija"));	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Assets/Vasija.Vasija"));
	if (MeshAsset.Succeeded()) {
		vasijaMesh->SetStaticMesh(MeshAsset.Object);
		vasijaMesh->SetWorldScale3D(FVector(0.8f));
		
	}
	vasijaMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AVasija::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVasija::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRelativeRotation(GetActorRotation() + FRotator(0.f, angulo, angulo));
}


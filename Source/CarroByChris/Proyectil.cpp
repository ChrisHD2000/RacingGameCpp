// Fill out your copyright notice in the Description page of Project Settings.


#include "Proyectil.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AProyectil::AProyectil()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Use a sphere as a simple collision representation.
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProyectil::OnHit);
	
	// Set the sphere's collision radius.
	CollisionComponent->InitSphereRadius(15.0f);
	// Set the root component to be the collision component.
	RootComponent = CollisionComponent;


	// Use this component to drive this projectile's movement.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.5f;
	// Die after 3 seconds.
	InitialLifeSpan = 5.0f;

	// Crea ell componente de sonido
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/SoundFXs/cocoLanzando_Cue.cocoLanzando_Cue"));
	if (SoundCue.Succeeded()) {
		SonidoLanzar = CreateDefaultSubobject<UAudioComponent>(TEXT("SuperVelocidad"));
		SonidoLanzar->SetSound(SoundCue.Object);
		SonidoLanzar->SetupAttachment(RootComponent);
	}
	SonidoLanzar->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AProyectil::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AProyectil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProyectil::FireInDirection(const FVector& ShootDirection)
{
	SonidoLanzar->Play();
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProyectil::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
}

void AProyectil::VelocidadInicial(FVector VelocidadCarro)
{
	ProjectileMovementComponent->InitialSpeed = VelocidadCarro.Size() + 3500.0f;
	ProjectileMovementComponent->MaxSpeed = VelocidadCarro.Size() + 3500.0f;
}

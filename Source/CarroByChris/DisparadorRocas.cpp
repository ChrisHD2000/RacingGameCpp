// Fill out your copyright notice in the Description page of Project Settings.


#include "DisparadorRocas.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Proyectil.h"
// Sets default values
ADisparadorRocas::ADisparadorRocas()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/SoundFXs/Risa_Cue.Risa_Cue"));
	if (SoundCue.Succeeded()) {
		sonido = CreateDefaultSubobject<UAudioComponent>(TEXT("Risa"));
		sonido->SetSound(SoundCue.Object);
		RootComponent = sonido;
	}
	sonido->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ADisparadorRocas::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle tiempo;
	GetWorldTimerManager().SetTimer(tiempo, this, &ADisparadorRocas::Fire, 1.18f, true);
	sonido->Play();
}

// Called every frame
void ADisparadorRocas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADisparadorRocas::Fire() {
	UE_LOG(LogTemp, Warning, TEXT("Fire rocks"));
	if (ProjectileClass )
	{
		float rot = FMath::RandRange(50.f, 85.f);
		// Get the camera transform.
		FRotator MuzzleRotation = FRotator(90.f,rot, 20.f);
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			// Spawn the projectile at the muzzle.
			AProyectil* Projectile = World->SpawnActor<AProyectil>(ProjectileClass, GetActorLocation() + FVector(0.f, 0.f, 150.f), MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				FRotator Direccion = GetActorRotation() + FRotator(0.f, rot, 0.f);
				Projectile->VelocidadInicial(GetVelocity());
				Projectile->FireInDirection(Direccion.Vector());

			}
		}
	}
}
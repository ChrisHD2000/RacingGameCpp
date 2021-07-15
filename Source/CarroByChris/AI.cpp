// Fill out your copyright notice in the Description page of Project Settings.


#include "AI.h"
#include "MyVehicleWheelFront.h"
#include "MyVehicleWheelRear.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "SkeletalRenderPublic.h"
#include "Components/SkinnedMeshComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Proyectil.h"
AAI::AAI() {

	//Ponemos al personaje

	Personaje = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Personaje"));
	Personaje->SetupAttachment(RootComponent);
	Personaje->SetWorldScale3D(FVector(1.0f));
	

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMeshAI(TEXT("/Game/CarroPartes/CarroChasisEjesCorrectosEscalado.CarroChasisEjesCorrectosEscalado"));
	GetMesh()->SetSkeletalMesh(CarMeshAI.Object);
	
	
	coberturaAI = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mi Cobertura"));
	coberturaAI->SetupAttachment(RootComponent);


	// creación y ubicación del volante 
	volante = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mi Volante"));
	volante->SetupAttachment(RootComponent);
	volante->SetRelativeLocation(FVector(16.f, 0.f, 55.f));
	volante->SetRelativeRotation(FRotator(-47.681f, 0.f, -90.f));
	volante->bReceivesDecals = false;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAssetAI2(TEXT("/Game/CarroPartes/Volante/VolanteOrigen.VolanteOrigen"));
	if (MeshAssetAI2.Succeeded()) {
		volante->SetStaticMesh(MeshAssetAI2.Object);
		volante->SetWorldScale3D(FVector(1.0f));
	}

	//Añadimos el Blueprint de animación
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/CarroPartes/AnimFinal"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	GetMesh()->bReceivesDecals = false;
	//Creamos el vehículo 4w

	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	//UKartMoveComponentAI * Vehicle4W = CastChecked<UKartMoveComponentAI>(GetVehicleMovement());
	check(Vehicle4W->WheelSetups.Num() == 4);
	// Setup the wheels
	Vehicle4W->WheelSetups[0].WheelClass = UMyVehicleWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("PhysWheel_FL");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -15.f, -1.f);

	Vehicle4W->WheelSetups[1].WheelClass = UMyVehicleWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("PhysWheel_FR");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 15.f, -1.f);

	Vehicle4W->WheelSetups[2].WheelClass = UMyVehicleWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("PhysWheel_BL");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -17.f, -1.f);

	Vehicle4W->WheelSetups[3].WheelClass = UMyVehicleWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("PhysWheel_BR");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(00.f, 17.f, -1.f);

	// Adjust the tire loading
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 4.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 4.0f;

	// Engine 
	// Torque setup
	Vehicle4W->MaxEngineRPM = 12500.0f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 6000.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(500.f, 9000.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(12000.f, 9500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(12500.f, 6000.f);




	// Adjust the steering 
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(80.0f, 0.8f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(300.0f, 0.1f);

	// Transmission	
	// We want Front Drive
	//Permite al carro poner su potencia hacia el frente, permitiendonos curvar con mayor rigidez
	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_FrontDrive;
	//Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;

	// Drive the front wheels a little more than the rear
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.63f;

	// Automatic gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;

	UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(Vehicle4W->UpdatedComponent);
	if (UpdatedPrimitive)
	{
		UpdatedPrimitive->BodyInstance.COMNudge = FVector(-25.f, 0.f, -16.f);
	}

	// Set the inertia scale. This controls how the mass of the vehicle is distributed.
	// Qué tan dificl es girar el carro respecto a los ejes x, y , z
	Vehicle4W->InertiaTensorScale = FVector(20.0f, 10.f, 1.2f * 1.7f);


	// Instanciamos el compente de AI llamado PawnSensingComp haciendolo un componente de nuestro carrito
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	
	// Crea los componentes de sonido
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/SoundFXs/superVelocidad_Cue.superVelocidad_Cue"));
	if (SoundCue.Succeeded()) {
		SuperVelocidad = CreateDefaultSubobject<UAudioComponent>(TEXT("SuperVelocidad"));
		SuperVelocidad->SetSound(SoundCue.Object);
		SuperVelocidad->SetupAttachment(GetMesh());
	}
	SuperVelocidad->bAutoActivate = false;


	
}

void AAI::Tick(float Delta) {
	Super::Tick(Delta);
	
}
void AAI::BeginPlay() {
	Super::BeginPlay();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AAI::OnPawnSeen);
}
/*
void AAI::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAI::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAI::MoveRight);
	PlayerInputComponent->BindAxis("RotateSteeringWheel", this, &AAI::RotarVolante);
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AAI::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AAI::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("RetornoVolante", IE_Released, this, &AAI::RegresoVolante);

}*/

void AAI::MoveForward(float Val)
{

	GetVehicleMovementComponent()->SetThrottleInput(Val);

}

void AAI::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void AAI::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AAI::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AAI::RotarVolante(float val) {

	if (volante->GetRelativeRotation().Roll > (roll - angMax) &&
		volante->GetRelativeRotation().Roll < (roll + angMax))
	{
		volante->SetRelativeRotation(volante->GetRelativeRotation() + FRotator(0.f, 0.f, 2.5f * val));
	}
	else {
		if (volante->GetRelativeRotation().Roll < (roll - angMax)) {
			volante->SetRelativeRotation(FRotator(pitch, 0.f, roll + 6.f - angMax));
		}
		if (volante->GetRelativeRotation().Roll > (roll + angMax)) {
			volante->SetRelativeRotation(FRotator(pitch, 0.f, roll - 6.f + angMax));
		}
	}

}

void AAI::RegresoVolante() {
	volante->SetRelativeRotation(FRotator(pitch, 0.f, roll));
}

void AAI::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr) {
		return;
	}	
	Fire();
}

void AAI::RecibirGolpe()
{
	UE_LOG(LogTemp, Warning, TEXT("OUCH"))
		
		GetMesh()->SetPhysicsLinearVelocity(FVector(0.f,0.f,1500.f));
}
void AAI::RecibirPiedrazo()
{
	UE_LOG(LogTemp, Warning, TEXT("OUCH"))
		GetMesh()->SetPhysicsLinearVelocity(FVector(0.f, 0.f, 2500.f));
	//GetMesh()->SetPhysicsAngularVelocityInRadians(FVector(0.f, 0.f, 10.f));
}
void AAI::UsePowerUp()
{	
		SuperVelocidad->Play();
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
		//FVector direccion = 4500.f * PlayerViewPointRotation.Vector();
		FVector direccion = GetActorForwardVector();
		UE_LOG(LogTemp, Warning, TEXT("La direccion es: %s"), *direccion.ToString())
		GetMesh()->SetPhysicsLinearVelocity(direccion*4500);	
		PawnSensingComp->bOnlySensePlayers = false;
}

void AAI::Fire()
{
	// Attempt to fire a projectile.
	if (ProjectileClass)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
		FRotator MuzzleRotation = CameraRotation;
		// Skew the aim to be slightly upwards.
		MuzzleRotation.Pitch += 10.0f;
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
				FRotator Direccion = GetActorRotation() + FRotator(7.5f, 0.f, 0.f);
				Projectile->VelocidadInicial(GetVelocity());
				Projectile->FireInDirection(Direccion.Vector());

			}
		}
	}
}
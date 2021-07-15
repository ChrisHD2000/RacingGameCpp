// Fill out your copyright notice in the Description page of Project Settings.


#include "WariPawn.h"
#include "MyVehicleWheelFront.h"
#include "MyVehicleWheelRear.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "SkeletalRenderPublic.h"
#include "Components/SkinnedMeshComponent.h"
#include "TimerManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Proyectil.h"
AWariPawn::AWariPawn() {

	//A penas se crea el personaje, se posee
	AutoPossessPlayer = EAutoReceiveInput::Player0;
////////////////////////// MALLA DEL MECANISMO DEL CARRO //////////////////////////////
	
	//cargamos la malla del mecanismo del carro
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/CarroPartes/CarroChasisEjesCorrectosEscalado.CarroChasisEjesCorrectosEscalado"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);


////////////////////////// MALLA DEL PERSONAJE DEL CARRO //////////////////////////////
	
	//Creamos la malla estatica para poner al personaje
	UStaticMeshComponent* Personaje = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Personaje"));
	Personaje->SetupAttachment(RootComponent);
	
	// Buscamos a la malla del personaje 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PersonajeMesh(TEXT("/Game/Assets/MallasPersonajes/Jaguar/JaguarStatic.JaguarStatic"));
	
	// Si se cargó con exito, añadimos esta malla al carro
	if (PersonajeMesh.Succeeded()) {
		Personaje->SetStaticMesh(PersonajeMesh.Object);
		Personaje->SetWorldScale3D(FVector(1.0f));
		Personaje->bReceivesDecals = false;
	}


////////////////////////// MALLA DE LA COBERTURA Y VOLANTE DEL CARRO //////////////////////////////
	
	// Buscamos a la malla de la cobertura del chasis del carro y la asignamos a cobertura
	UStaticMeshComponent* cobertura = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mi Cobertura"));
	cobertura->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/CarroPartes/Cobertura12.Cobertura12"));
	
	// Si se cargó con exito, añadimos esta malla al carro
	if (MeshAsset.Succeeded()) {
		cobertura->SetStaticMesh(MeshAsset.Object);
		cobertura->SetWorldScale3D(FVector(1.0f));
		cobertura->bReceivesDecals = false;
	}
	
	// creación y ubicación del volante 
	volante = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mi Volante"));
	volante->SetupAttachment(RootComponent);
	volante->SetRelativeLocation(FVector(16.f, 0.f, 55.f));
	volante->SetRelativeRotation(FRotator(-47.681f, 0.f, -90.f));
	volante->bReceivesDecals = false;
	//Buscamos la malla del volante
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset2(TEXT("/Game/CarroPartes/Volante/VolanteOrigen.VolanteOrigen"));
	if (MeshAsset2.Succeeded()) {
		volante->SetStaticMesh(MeshAsset2.Object);
		volante->SetWorldScale3D(FVector(1.0f));
	}
	roll = volante->GetRelativeRotation().Roll; //ángulo para rotacion del volante en eje z
	pitch = volante->GetRelativeRotation().Pitch; // ángulo para rotación del volante en eje x


////////////////////////// BLUEPRINT DE ANIMACIÓN ASOCIADO AL MECANISMO DEL CARRO //////////////////////////////
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/CarroPartes/AnimFinal"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	GetMesh()->bReceivesDecals = false;

	////////////////////////////// COMPONENTE DE SISTEMA DE PARTICULAS //////////////////////////////////////
	burbujas = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Parituculas"));
	burbujas->SetupAttachment(RootComponent);
	burbujas->SetRelativeLocation(FVector(-105.67f, -15.628f, 41.1371f));
	burbujas->SetRelativeRotation(FRotator(10.f, 50.000023f, 0.f));

	burbujas2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Parituculas2"));
	burbujas2->SetupAttachment(RootComponent);
	burbujas2->SetRelativeLocation(FVector(-105.67f, 15.628f, 41.1371f));
	burbujas2->SetRelativeRotation(FRotator(-10.f, 50.000023f, 0.f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> BurbujasSistema(TEXT("/Game/Materiales/BurbujasParticulas.BurbujasParticulas"));
	if (BurbujasSistema.Succeeded()) {
		burbujas->SetTemplate(BurbujasSistema.Object);
		burbujas2->SetTemplate(BurbujasSistema.Object);

	}
////////////////////////// COMPONENTE DE MOVIMIENTO DEL VEHÍCULO //////////////////////////////

	//Creamos una instancia de UWheeledVehicleMovementComponent4W llamada Vehicle4W para poder usar el componente
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	check(Vehicle4W->WheelSetups.Num() == 4);


	// Inicializamos las llantas haciendo referencia a nuestras clases UMyVehicleWheelFront y UMyVehicleWheelRear
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

	// Ajustamos el peso de las llantas
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 4.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 4.0f;

	// Iniciallizamos el motor y la curva del torque del vehículo
	Vehicle4W->MaxEngineRPM = 12500.0f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 6000.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(500.f, 9000.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(12000.f, 9500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(12500.f, 6000.f);

	// Ajustamos el steering 
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(80.0f, 0.8f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(300.0f, 0.1f);

	//Permite al carro poner su potencia hacia el frente, permitiendonos curvar con mayor rigidez
	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_FrontDrive;

	// Arranca las ruedas delanteras con una intensidad un poco mayor a las traseras
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.63f;

	// Caja de cambios automática
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;

	// Ajustamos el centro de masa del vehiculo
	UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(Vehicle4W->UpdatedComponent);
	if (UpdatedPrimitive)
	{
		UpdatedPrimitive->BodyInstance.COMNudge = FVector(-25.f, 0.f, -16.f);
	}

	// Ponemos qué tan dificl es girar el carro respecto a los ejes x, y , z
	Vehicle4W->InertiaTensorScale = FVector(10.0f, 10.f, 0.1f);

////////////////////////// BRAZO DE RESORTE DE LA CÁMARA //////////////////////////////

	// Crea el brazo de resorte para perseguir la cámara
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(-300.f, 0.f, 180.f));
	SpringArm->SetWorldRotation(FRotator(-5.f, 0.0f, 0.0f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 3.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;
	
	//Crea el brazo de resorte para la camara trasera
	SpringArm2 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm2"));
	SpringArm2->SetRelativeLocation(FVector(200.f, 0.f, 100.f));
	SpringArm2->SetWorldRotation(FRotator(-5.f, 180.0f, 0.F));
	SpringArm2->SetupAttachment(RootComponent);
	SpringArm2->TargetArmLength = 100.f;

	//Crea la cámara que va a perseguir al personaje
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Camera->SetRelativeRotation(FRotator(0.0f, .0f, 0.0f));
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	//Crea la camara trasera
	camTrasera = CreateDefaultSubobject<UCameraComponent>(TEXT("CamaraTrasera"));
	camTrasera->SetupAttachment(SpringArm2, USpringArmComponent::SocketName);

	// Crea ell componente de sonido
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/SoundFXs/superVelocidad_Cue.superVelocidad_Cue"));
	if (SoundCue.Succeeded()) {
		SuperVelocidad = CreateDefaultSubobject<UAudioComponent>(TEXT("SuperVelocidad"));
		SuperVelocidad->SetSound(SoundCue.Object);
		SuperVelocidad->SetupAttachment(GetMesh());
	}
	SuperVelocidad->bAutoActivate = false;
}

void AWariPawn::Tick(float Delta) {
	Super::Tick(Delta);
}

void AWariPawn::BeginPlay() {
	Super::BeginPlay();
	FTimerHandle tiempo;
	GetWorldTimerManager().SetTimer(tiempo, this, &AWariPawn::ComenzarCarrera, 4.f);
}

void AWariPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Enlazamos las entradas de teclado con sus métodos
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AWariPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWariPawn::MoveRight);
	PlayerInputComponent->BindAxis("RotateSteeringWheel", this, &AWariPawn::RotarVolante);
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AWariPawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AWariPawn::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("RetornoVolante", IE_Released, this, &AWariPawn::RegresoVolante);
	PlayerInputComponent->BindAction("CambiarCamara", IE_Pressed, this, &AWariPawn::CambiarCamara);
	PlayerInputComponent->BindAction("CambiarCamara", IE_Released, this, &AWariPawn::RetornarCamara);
	PlayerInputComponent->BindAction("PowerUp", IE_Released, this, &AWariPawn::UsePowerUp);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AWariPawn::Fire);
}

void AWariPawn::MoveForward(float Val)
{
	if (comenzo)
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}
void AWariPawn::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}
void AWariPawn::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AWariPawn::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AWariPawn::RotarVolante(float val) {

	// Necesitamos que el volante gire tanto como las llantas
	// en este caso roll es un numero negativo ya que se requirió para que el volante no quede desfasado
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
void AWariPawn::RegresoVolante() {
		volante->SetRelativeRotation(FRotator(pitch, 0.f, roll));
}
void AWariPawn::CambiarCamara() {
		Camera->Deactivate();
		camTrasera->Activate();
}

void AWariPawn::RetornarCamara(){
		camTrasera->Deactivate();
		Camera->Activate();
}
void AWariPawn::ComenzarCarrera()
{
	comenzo = true;
}
void AWariPawn::UsePowerUp()
{
	if (comenzo && tiros > 0) {
		tiros--;
		SuperVelocidad->Play();
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
		FVector direccion = 4500.f * PlayerViewPointRotation.Vector();
		UE_LOG(LogTemp, Warning, TEXT("La direccion es: %s"), *direccion.ToString())
		GetMesh()->SetPhysicsLinearVelocity(direccion);
	}
}
void AWariPawn::Fire()
{
	// Attempt to fire a projectile.
	if (ProjectileClass && comenzo)
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
void AWariPawn::RecibirGolpe()
{
	UE_LOG(LogTemp, Warning, TEXT("OUCH"))
		GetMesh()->SetPhysicsLinearVelocity(FVector(0.f, 0.f, 1500.f));
}

void AWariPawn::RecibirPiedrazo()
{
	UE_LOG(LogTemp, Warning, TEXT("OUCH"))
		GetMesh()->SetPhysicsLinearVelocity(FVector(0.f, 0.f, 2500.f));
	GetMesh()->SetPhysicsAngularVelocityInRadians(FVector(0.f, 0.f, 10.f));
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Proyectil.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UAudioComponent;
UCLASS()
class CARROBYCHRIS_API AProyectil : public AActor
{
	GENERATED_BODY()
	
public:	
	// Constructor para configurar valores por defecto del proyectil
	AProyectil();
	// Componente de colisión esférico
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;
	// Componente de movimiento de proyectil
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditAnywhere)
		UAudioComponent* SonidoLanzar;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Función que iniciaiza la velocidad en la dirección del disapro
	void FireInDirection(const FVector& ShootDirection);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	void VelocidadInicial(FVector VelocidadCarro);
};

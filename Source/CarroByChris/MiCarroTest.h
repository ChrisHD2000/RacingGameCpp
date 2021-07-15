// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "MiCarroTest.generated.h"

//class UPhysicallMaterial;
class UCameraComponent;
class USpringArmComponent;
class UInputComponent;
class UStaticMeshComponent;
class USceneComponent;
class UAudioComponent;
class UParticleSystemComponent;

UCLASS()
class CARROBYCHRIS_API AMiCarroTest : public AWheeledVehicle
{
	GENERATED_BODY()

public:
		UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm;
		UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm2;
		UPROPERTY(EditAnywhere)
		UCameraComponent* Camera;
		UPROPERTY(EditAnywhere)
		UCameraComponent* camTrasera;
		UPROPERTY(EditAnywhere)
		UStaticMeshComponent* volante;
		UPROPERTY(EditAnywhere)
		UParticleSystemComponent* burbujas;
		UPROPERTY(EditAnywhere)
		UParticleSystemComponent* burbujas2;
		UPROPERTY(EditAnywhere)
		UAudioComponent* SuperVelocidad;	

public: 
	AMiCarroTest();
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void Tick(float Delta) override;		
	bool bInReverseGear;
	float angMax = 60.f;
	float roll, pitch;
	int PersonajeID = 1;
	bool comenzo = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CheckPoint = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Lap = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float refPos = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Lugar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int tiros = 3;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProyectil> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int lugarFinal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Nombre;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int puntuacion;
protected:
	virtual void BeginPlay() override;
public:
	void MoveForward(float val);
	void MoveRight(float val);
	void OnHandbrakePressed();
	void OnHandbrakeReleased();	
	void CambiarCamara();
	void RegresoVolante();
	void RotarVolante(float val);
	void RetornarCamara();
	void ComenzarCarrera();
	void UsePowerUp();
	UFUNCTION(BlueprintCallable)
	void RecibirPiedrazo();
	UFUNCTION(BlueprintCallable)
		void RecibirGolpe();
	UFUNCTION()
		void Fire();
	/** Returns SpringArm subobject **/
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
};

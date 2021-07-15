// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "AI.generated.h"


class UPhysicallMaterial;
class UCameraComponent;
class USpringArmComponent;
class UInputComponent;
class UStaticlMeshComponent;
class USceneComponent;
class UPawnSensingComponent;
class UAudioComponent;
UCLASS()
class CARROBYCHRIS_API AAI : public AWheeledVehicle
{
	GENERATED_BODY()
	public:		
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* volante;
	public:
	AAI();
	//virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void Tick(float Delta) override;
	bool bInReverseGear;
	float angMax = 60.f;
	float roll, pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int idMalla;
	UPROPERTY(EditAnywhere)
		UAudioComponent* SuperVelocidad;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProyectil> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = OtroBot)
		TSubclassOf<class AAI> BotClass;
protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintCallable)
	void MoveForward(float val);	
	UFUNCTION(BlueprintCallable)
	void MoveRight(float val);
	UFUNCTION(BlueprintCallable)
	void OnHandbrakePressed();
	UFUNCTION(BlueprintCallable)
	void OnHandbrakeReleased();
	UFUNCTION(BlueprintCallable)
		void UsePowerUp();
	UFUNCTION(BlueprintCallable)
		void RecibirGolpe();
	UFUNCTION(BlueprintCallable)
		void RecibirPiedrazo();
	void RotarVolante(float val);
	void RegresoVolante();
	//TArray<ATargetPoint*> TargetPoints;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Personaje;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* coberturaAI;
	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);
	UFUNCTION()
	void Fire();
public:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UPawnSensingComponent* PawnSensingComp;
};

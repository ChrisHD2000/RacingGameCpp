// Fill out your copyright notice in the Description page of Project Settings.


#include "MyVehicleWheelFront.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"
UMyVehicleWheelFront::UMyVehicleWheelFront() {

	// Define el radio de los neumáticos frontales
	ShapeRadius = 19.911f;
	// Define el espesor de los neumáticos frontales
	ShapeWidth = 17.332f ;
	// Define si los neumáticos frontales se ven afectados por el freno de mano o no
	bAffectedByHandbrake = false;
	// Define el ángulo de giro máximo de los neumáticos frontales
	SteerAngle = 60.f;

	// Define desde dónde en el eje vertical se aplican las fuerzas de suspensión.
	SuspensionForceOffset = -2.0f;
	// Qué tan lejos los neumaticos pueden ascender por encima del punto de descanso.
	SuspensionMaxRaise = 8.0f;
	// Qué tan lejos los neumaticos pueden descender por debajo del punto de descanso.
	SuspensionMaxDrop = 6.0f;
	//Frecuencia con que el sistema tiende a oscilar en la ausencia de cualquier fuerza externa 
	SuspensionNaturalFrequency = 1.f;
	//Determina cómo oscilan las llantas del vehículo después de una alteración durante su camino
	SuspensionDampingRatio = 30.5f;

	//Carga máxima normalizada del neumático a la que no se le puede ofrecer más rigidez
	LatStiffMaxLoad = 10.3f;
	//Cuanta rigidez lateral proporciona el deslizamiento lateral
	LatStiffValue = 50.f;
	//Cuanta rigidez longitudinal proporciona el deslizammiento longitudinal
	LongStiffValue = 5000.f;


	//Se llama al material físico de los neumáticos para este neumático
	static ConstructorHelpers::FObjectFinder<UTireConfig> TireData(TEXT("/Game/CarroPartes/TireConfig/Front.Front"));
	TireConfig = TireData.Object;
}

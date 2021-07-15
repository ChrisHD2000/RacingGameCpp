// Fill out your copyright notice in the Description page of Project Settings.


#include "MyVehicleWheelFront.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"
UMyVehicleWheelFront::UMyVehicleWheelFront() {

	// Define el radio de los neum�ticos frontales
	ShapeRadius = 19.911f;
	// Define el espesor de los neum�ticos frontales
	ShapeWidth = 17.332f ;
	// Define si los neum�ticos frontales se ven afectados por el freno de mano o no
	bAffectedByHandbrake = false;
	// Define el �ngulo de giro m�ximo de los neum�ticos frontales
	SteerAngle = 60.f;

	// Define desde d�nde en el eje vertical se aplican las fuerzas de suspensi�n.
	SuspensionForceOffset = -2.0f;
	// Qu� tan lejos los neumaticos pueden ascender por encima del punto de descanso.
	SuspensionMaxRaise = 8.0f;
	// Qu� tan lejos los neumaticos pueden descender por debajo del punto de descanso.
	SuspensionMaxDrop = 6.0f;
	//Frecuencia con que el sistema tiende a oscilar en la ausencia de cualquier fuerza externa 
	SuspensionNaturalFrequency = 1.f;
	//Determina c�mo oscilan las llantas del veh�culo despu�s de una alteraci�n durante su camino
	SuspensionDampingRatio = 30.5f;

	//Carga m�xima normalizada del neum�tico a la que no se le puede ofrecer m�s rigidez
	LatStiffMaxLoad = 10.3f;
	//Cuanta rigidez lateral proporciona el deslizamiento lateral
	LatStiffValue = 50.f;
	//Cuanta rigidez longitudinal proporciona el deslizammiento longitudinal
	LongStiffValue = 5000.f;


	//Se llama al material f�sico de los neum�ticos para este neum�tico
	static ConstructorHelpers::FObjectFinder<UTireConfig> TireData(TEXT("/Game/CarroPartes/TireConfig/Front.Front"));
	TireConfig = TireData.Object;
}

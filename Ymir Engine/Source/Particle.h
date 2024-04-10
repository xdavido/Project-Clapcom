#pragma once

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "Color.h"
#include "CMaterial.h"

class Particle
{
public:
	Particle();
	~Particle();

	//Tranform
	float3 position;
	Quat worldRotation; //Para el billboarding
	float3 size;
	float4 velocity; // (x,y,z son los vectores, normalizados, w es la intensidad de la velocidad)

	//Lifetime (time)
	float lifetime; //Variable % de su vida 
	float oneOverMaxLifetime; //Vida total en 1.0f/dt que queremos que dure, siendo el dt su duracion (60 dt = 1 segundo aprox?)
	
	//"Lifetime" (distance)
	bool diesByDistance; //When true, if distance limit is lower than actual distance kill particle
	float3 initialPosition; //This is for killing by distance
	float distanceLimit;

	//Material 
	Color color;
	CMaterial* mat = nullptr;

private:
};

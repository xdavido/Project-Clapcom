#include "Particle.h"

Particle::Particle()
{
	position = { 0,0,0 };
	worldRotation = { 0,0,0,0 };
	size = { 1,1,1 };
	velocity = { 0,0,0,0 };

	lifetime = 0.0f;
	oneOverMaxLifetime = 1.0f / 60;

	diesByDistance = false;
	initialPosition = {0,0,0}; //This is for killing by distance
	distanceLimit = 0.0f;
	
	color = { 255,255,255,255 };
}

Particle::~Particle()
{
}

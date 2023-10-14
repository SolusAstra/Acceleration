#pragma once
#include "Primitive.h"

/* It might be easiest to define multiple particle types Particle2, Particle3
 * and then use a template to define Particle<T> where T is either Particle2 or Particle3
 * * This way, we can use the same BVH class for both 2D and 3D particles
*/

template <typename T>
class Particle : public Primitive<T> {

public:
	// Constructors
	Particle() : Primitive<T>(PARTICLE, std::vector<T>()) {}

	Particle(ACCEL_INPUT_FORMAT format, std::vector<T> vertex) 
		: Primitive<T>(PARTICLE, format, vertex) {}

	Particle(std::vector<T> vertex, std::vector<int> idx) : Primitive<T>(PARTICLE, vertex, idx) {}

};
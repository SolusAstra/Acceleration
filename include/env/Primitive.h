#pragma once
#include <vector>
#include <sutil\vec_math.h>
#include <numeric>

#include "Ray.h"
#include "AABB.h"

enum ACCEL_INPUT_TYPE {
	CUSTOM = 0,
    SPHERE = 1,
    TRIANGLE = 2,
    PARTICLE = 3,
    PARTICLE_CUSTOM = 4
};

class dPrimitive {
public:
    ACCEL_INPUT_TYPE type;
    float3* vertex; // Assume externally managed
    int3* face;     // Assume externally managed
    size_t N;       // Number of vertices
    size_t F;       // Number of faces

    dPrimitive() : vertex(nullptr), face(nullptr), N(0), F(0) {}

    virtual ~dPrimitive() {
        delete[] vertex;
        delete[] face;
    }

    __host__ __device__ virtual bool hit(int idx, const Trace::Ray& ray, float t_max, float& root) const = 0;

};

class Primitive {

public:

	ACCEL_INPUT_TYPE type = ACCEL_INPUT_TYPE::CUSTOM;
	size_t N = 0;
    std::vector<float3> vertex = std::vector<float3>();
    std::vector<int3> face = std::vector<int3>();
    
public:

    virtual bool hit(int idx, const Trace::Ray& ray, float t_max, float& root) const = 0;
    virtual AABB surrounding(int objID) const = 0;
    virtual Primitive* reduceToPrimitive() const = 0;
    virtual dPrimitive* createDeviceVersion() const = 0;

};


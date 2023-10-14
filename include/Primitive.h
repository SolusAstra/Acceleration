#pragma once
#include <vector>
#include <sutil\vec_math.h>
#include <numeric>

enum ACCEL_INPUT_TYPE {
	CUSTOM = 0,
    SPHERE = 1,
    TRIANGLE = 2,
    PARTICLE = 3,
    PARTICLE_CUSTOM = 4
};

enum ACCEL_INPUT_FORMAT {
    NONE = 0,
    FLOAT2 = 1,
    FLOAT3 = 2,
};

template <typename T>
class Primitive {

public:

	// Primitive identifiers
	ACCEL_INPUT_TYPE type = ACCEL_INPUT_TYPE::CUSTOM;
    ACCEL_INPUT_FORMAT format = ACCEL_INPUT_FORMAT::NONE;

	// Primitive data
	size_t N;
    std::vector<T> vertex;
    std::vector<int> idx;
    

public:

	// Constructors
	inline Primitive()
	{
		this->type = ACCEL_INPUT_TYPE::CUSTOM;
		this->format = ACCEL_INPUT_FORMAT::NONE;
		this->N = 0;
		this->vertex = std::vector<T>();
		this->idx = std::vector<int>();
	}

	inline Primitive(ACCEL_INPUT_TYPE type, ACCEL_INPUT_FORMAT format, const std::vector<T>& vertex)
	{
		this->type = type;
		this->format = format;
		this->N = vertex.size();
		this->vertex = vertex;
		this->idx = std::vector<int>(N);
		std::iota(idx.begin(), idx.end(), 0);
	}

	inline Primitive(ACCEL_INPUT_TYPE type, const std::vector<T>& vertex, const std::vector<int>& idx)
	{
		this->type = type;
		this->format = ACCEL_INPUT_FORMAT::NONE;
		this->N = vertex.size();
		this->vertex = vertex;
		this->idx = idx;
	}

};
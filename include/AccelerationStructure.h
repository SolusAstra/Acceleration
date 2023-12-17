#pragma once
#include "bvh/Bvh.h"
#include "BruteForce.h"

enum ACCEL_TYPE {
	ACCEL_NONE,
	ACCEL_BVH,
	ACCEL_KD_TREE
};

enum ACCEL_FORMAT {
	FLOAT2,
	FLOAT3
};

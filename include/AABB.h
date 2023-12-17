#pragma once
#include <vector>
#include <sutil\vec_math.h>

class AABB {
public:
    float3 min = make_float3(0.0f);
    float3 max = make_float3(0.0f);

	AABB() {}
    AABB(const float3& vertex) : min(vertex), max(vertex) {}
	AABB(const float3& min, const float3& max) : min(min), max(max) {}

};


inline int longestAxis(const AABB& box) {
	// Compute longest axis of a bounding box (specific to float 3)
	float3 diff = box.max - box.min;
	if (diff.x >= diff.y && diff.x >= diff.z) {
		return 0; // x-axis is longest
	}
	else if (diff.y >= diff.x && diff.y >= diff.z) {
		return 1; // y-axis is longest
	}
	else {
		return 2; // z-axis is longest
	}

}

// Create a bounding box that surrounds a single point
inline AABB surrounding(const float3& point) {
	AABB box(point);
	return box;
}

inline AABB surrounding(const AABB& boxA, const AABB& boxB) {
	return AABB(fminf(boxA.min, boxB.min), fmaxf(boxA.max, boxB.max));	
}

// Create a bounding box that surrounds two points
inline AABB surrounding(const std::vector<float3>& points) {
	AABB box;
	box.min = points[0];
	box.max = points[0];
	for( int i = 1; i < points.size(); i++ ) {
		box.min = fminf(box.min, points[i]);
		box.max = fmaxf(box.max, points[i]);
	}
	return box;
}

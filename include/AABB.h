#pragma once
#include <sutil\vec_math.h>
#include <vector>

class AABBBase {
public:
	virtual ~AABBBase() {}
	// Define common interface for AABB operations
};

template <typename T>
class AABB : public AABBBase {
public:
	T min;
	T max;

	AABB() {}
	AABB(const T& min, const T& max) : min(min), max(max) {}
};

template <typename T>
inline int longestAxis(const AABB<T>& box) {
	// Compute longest axis of a bounding box (specific to float 2)
	T diff = box.max - box.min;
	if (diff.x >= diff.y) {
		return 0; // x-axis is longest
	}
	else {
		return 1; // y-axis is longest
	}

}

template <>
inline int longestAxis(const AABB<float3>& box) {
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
template <typename T>
inline AABB<T> surrounding(const T& point) {
	AABB<T> box(point, point);
	return box;
}

template <typename T>
inline AABB<T> surrounding(const AABB<T>& boxA, const AABB<T>& boxB) {
	return AABB<T>(fminf(boxA.min, boxB.min), fmaxf(boxA.max, boxB.max));	
}

// Create a bounding box that surrounds two points
template <typename T>
inline AABB<T> surrounding(const std::vector<T>& points) {
	AABB<T> box;
	box.min = points[0];
	box.max = points[0];
	for( int i = 1; i < points.size(); i++ ) {
		box.min = fminf(box.min, points[i]);
		box.max = fmaxf(box.max, points[i]);
	}
	return box;
}

// Create a bounding box that surrounds two points
template <typename T>
inline AABB<T> surrounding(const std::vector<T>& points, std::vector<int> subset) {
	AABB<T> box(points[subset[0]], points[subset[0]]);
	for (int i = 1; i < subset.size(); i++) {
		box = surrounding(box, points[subset[i]]);
	}
	return box;
}



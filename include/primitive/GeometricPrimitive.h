#pragma once
#include "Centroid.h"
#include "AABB.h"

class GeometricPrimitive {

public:
    std::vector<float3> vertex = std::vector<float3>();
    std::vector<int3> face = std::vector<int3>();

    GeometricPrimitive() {}
    GeometricPrimitive(const std::vector<float3>& vtx) : vertex(vtx) {}
    GeometricPrimitive(const std::vector<float3>& vtx, const std::vector<int3>& idx) : vertex(vtx), face(idx) {}

    virtual AABB surrounding(int objID) const = 0;
    virtual Centroid computeCentroid() const = 0;
};
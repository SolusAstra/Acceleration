#pragma once
#include "primitive/GeometricPrimitive.h"

float3 computeBarycenter(const std::vector<float3>& vtx, const std::vector<int3>& idx, int objID) {
    float3 A = vtx[idx[objID].x];
    float3 B = vtx[idx[objID].y];
    float3 C = vtx[idx[objID].z];
    return (A + B + C) / 3.0f;
}

float3 computeNormal(const std::vector<float3>& vtx, const std::vector<int3>& idx, int objID) {
    float3 A = vtx[idx[objID].x];
    float3 B = vtx[idx[objID].y];
    float3 C = vtx[idx[objID].z];
    float3 edge1 = normalize(B - A);
    float3 edge2 = normalize(C - A);

    return normalize(cross(edge2, edge1));
}

class Triangle : public GeometricPrimitive {

public:

    Triangle() : GeometricPrimitive() {}
    Triangle(const std::vector<float3>& vtx, const std::vector<int3>& idx) : GeometricPrimitive(vtx, idx) {}

    Centroid computeCentroid() const override {

        std::vector<float3> barycenter(face.size());
        for (int k = 0; k < face.size(); k++) {
            barycenter[k] = computeBarycenter(vertex, face, k);
        }

        return Centroid(barycenter);
    }

    virtual AABB surrounding(int objID) const override {
        AABB box;

        int3 triID = face[objID];

        box.min = vertex[triID.x];
        box.max = vertex[triID.x];

        box.min = fminf(box.min, vertex[triID.y]);
        box.max = fmaxf(box.max, vertex[triID.y]);

        box.min = fminf(box.min, vertex[triID.z]);
        box.max = fmaxf(box.max, vertex[triID.z]);

        return box;
    }

};
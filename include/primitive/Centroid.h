#pragma once
#include <vector>
#include <sutil/vec_math.h>

class Centroid {

public:
    std::vector<float3> vertex = std::vector<float3>();

    Centroid() {}
    Centroid(const std::vector<float3>& vtx) : vertex(vtx) {}
};
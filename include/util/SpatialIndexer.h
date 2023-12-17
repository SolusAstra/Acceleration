#pragma once
#include "sutil\vec_math.h"
#include <algorithm>
#include <vector>
#include <numeric>
#include <functional>


void sortAlongXX(const std::vector<float3>& vertex, std::vector<int>& indices)
{
    std::sort(indices.begin(), indices.end(), [&vertex](int a, int b) {
        return vertex[a].x < vertex[b].x;
        });
}

void sortAlongYY(const std::vector<float3>& vertex, std::vector<int>& indices)
{
    std::sort(indices.begin(), indices.end(), [&vertex](int a, int b) {
        return vertex[a].y < vertex[b].y;
        });
}

void sortAlongZZ(const std::vector<float3>& vertex, std::vector<int>& srtIdx)
{
    std::sort(srtIdx.begin(), srtIdx.end(), [&vertex](int a, int b) {
        return vertex[a].z < vertex[b].z;
        });
}

struct SpatialIndexer {
    std::vector<std::vector<int>> data;
    std::vector<std::vector<float>> val;
    int N;
    int M;

    SpatialIndexer() {}

    SpatialIndexer(int N, int M) : N(N), M(M) {

        data.resize(N);
        for (int i = 0; i < N; i++) {
            data[i].resize(M);
            std::iota(data[i].begin(), data[i].end(), 0);
        }
    }

    static SpatialIndexer rankVectors(const std::vector<float3>& v) {
        SpatialIndexer rankedIdx(3, v.size());
        rankedIdx.rank(v);
        return rankedIdx;
    }

    std::vector<int>& operator()(int axis) { return data[axis]; }
    int& operator()(int axis, int element) { return data[axis][element]; }

    void setData(const std::vector<float3>& v);
    void rank(const std::vector<float3>& v);
    void sort(const std::vector<float3>& v);

};

void SpatialIndexer::setData(const std::vector<float3>& vertex) {

    val.resize(N);
    for (int i = 0; i < N; i++) {
        val[i].resize(M);
    }

    // Get centroid of each primitive
    for (int i = 0; i < M; ++i) {
        val[0][i] = vertex[i].x;
        val[1][i] = vertex[i].y;
        val[2][i] = vertex[i].z;
    }

}

inline void SpatialIndexer::rank(const std::vector<float3>& v) {

    // Sort pitives
    sort(v);

    // Temporarily copy 
    SpatialIndexer temp(*this);
    for (int i = 0; i < M; ++i) {
        temp(0, this->operator()(0, i)) = i;
        temp(1, this->operator()(1, i)) = i;
        temp(2, this->operator()(2, i)) = i;
    }

    // Save ranked indices
    this->data.swap(temp.data);
    setData(v);
}

void SpatialIndexer::sort(const std::vector<float3>& v) {
    sortAlongXX(v, this->data[0]);
    sortAlongYY(v, this->data[1]);
    sortAlongZZ(v, this->data[2]);
}

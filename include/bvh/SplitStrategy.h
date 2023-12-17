#pragma once
#include "util/SpatialIndexer.h"
#include "util/rng.h"

struct MidCount {};
struct MidPoint {};

void transformIndices(std::vector<int>& subsetIdx, const int axis, SpatialIndexer& rankIdx) {

    std::vector<int>& rankedAxis = rankIdx.data[axis];

    // Sort subsetIdx directly using a custom comparison function
    std::sort(subsetIdx.begin(), subsetIdx.end(), [&rankedAxis](int a, int b)
        {
            return rankedAxis[a] < rankedAxis[b];
        });
}

template <typename T>
inline void split(std::vector<int>& L, std::vector<int>& R, std::vector<int>& subset, SpatialIndexer& rankIdx) {

}

template <>
inline void split<MidCount>(std::vector<int>& L, std::vector<int>& R, std::vector<int>& subset, SpatialIndexer& rankIdx) {

    // Sort subset indices along chosen axis
    int axis = getRandomAxis();
    transformIndices(subset, axis, rankIdx);

    auto midpoint = subset.begin() + subset.size() / 2;
    L.assign(subset.begin(), midpoint);
    R.assign(midpoint, subset.end());
}

template <>
inline void split<MidPoint>(std::vector<int>& L, std::vector<int>& R, std::vector<int>& subset, SpatialIndexer& rankIdx) {

    int maxAxis = -1;
    float maxVariance = 0;

    for (int axis = 0; axis <= 2; axis++) {

        transformIndices(subset, axis, rankIdx);

        float max_f = rankIdx.val[axis][subset[subset.size() - 1]];
        float min_f = rankIdx.val[axis][subset[0]];
        float variance = max_f - min_f;

        if (variance > maxVariance) {
            maxAxis = axis;
            maxVariance = variance;
        }
    }

    transformIndices(subset, maxAxis, rankIdx);

    auto midpoint = subset.begin() + subset.size() / 2;
    L.assign(subset.begin(), midpoint);
    R.assign(midpoint, subset.end());
}
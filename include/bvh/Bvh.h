#pragma once
#include "Node.h"
#include "primitive/GeometricPrimitive.h"

class BVH {

public:
    int depth = 0;
    size_t size = 0;
    std::vector<Node> node;
    std::vector<AABB> bbox;
    GeometricPrimitive* primitive;

public:

    BVH() {}
    ~BVH() {}

    BVH(GeometricPrimitive* prim) {

        int rootIdx = 0;

        //Centroid primitiveCentroid = prim->computeCentroid();
        SpatialIndexer rankIdx = SpatialIndexer::rankVectors(prim->computeCentroid().vertex);

        this->size = 2 * rankIdx.M - 1;
        this->node = std::vector<Node>(size);

        std::vector<int> subset(rankIdx.M);
        std::iota(subset.begin(), subset.end(), 0);

        Node::generateHierarchy<MidPoint>(this->node, rootIdx, subset, rankIdx);
    }
    
};
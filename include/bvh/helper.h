#pragma once
#include "Bvh.h"

void computeDepth(BVH* bvh, int& nodeIdx) {

    // Recursively dive into tree structure and set depth of each node.
    // Compute final depth of tree structure.
    int bvhDepth = 0;
    Node* nodePtr = &bvh->node[nodeIdx];

    if (nodePtr->isLeaf) {
        nodePtr->depth = 0;
        bvhDepth = 0;
    }
    else {
        int leftBranch = nodePtr->branchIdx[0];
        int rightBranch = nodePtr->branchIdx[1];
        computeDepth(bvh, leftBranch);
        computeDepth(bvh, rightBranch);

        // Set depth of current node to 1 + max(left, right)
        nodePtr->depth = 1 + std::max(bvh->node[leftBranch].depth, bvh->node[rightBranch].depth);

        bvhDepth = nodePtr->depth;
    }

    // Set depth of BVH to be maximum depth of tree structure
    bvh->depth = std::max(bvh->depth, bvhDepth);
}



void getPath(BVH* bvh, int& nodeIdx, bool& primFound, const int primID, std::vector<int>& path) {

    // Recursively dive into tree structure and set depth of each node.
    // Compute final depth of tree structure.
    Node* nodePtr = &bvh->node[nodeIdx];

    if (nodePtr->isLeaf) {

        if (nodePtr->primitiveIdx == primID) {

            primFound = true;
            path.push_back(nodeIdx);
            return;
        }

    }
    else {
        int leftBranch = nodePtr->branchIdx[0];
        int rightBranch = nodePtr->branchIdx[1];
        getPath(bvh, leftBranch, primFound, primID, path);

        if (primFound) {
            path.push_back(nodeIdx);
            return;
        }


        getPath(bvh, rightBranch, primFound, primID, path);

        if (primFound) {
            path.push_back(nodeIdx);
            return;
        }
    }


}

void computeBoundingBoxes_BVH(BVH* bvh, GeometricPrimitive* prim, int& nodeIdx) {

    Node* nodePtr = &bvh->node[nodeIdx];
    AABB* bboxPtr = &bvh->bbox[nodeIdx];

    if (nodePtr->isLeaf) {
        int primIdx = nodePtr->primitiveIdx;

        AABB* bounding = new AABB(prim->surrounding(primIdx));
        bboxPtr->min = bounding->min;
        bboxPtr->max = bounding->max;

        nodeIdx++;
    }
    else {
        // Construct bounding boxes for children
        for (int i = 0; i < 2; i++) {
            int branchIdx = nodePtr->branchIdx[i];
            computeBoundingBoxes_BVH(bvh, prim, branchIdx);
        }

        // Construct bounding box for parent
        int branchIdx0 = nodePtr->branchIdx[0];
        int branchIdx1 = nodePtr->branchIdx[1];

        // Get bounding boxes of children
        AABB* leftBox = &bvh->bbox[branchIdx0];
        AABB* rightBox = &bvh->bbox[branchIdx1];
        AABB* bounding = new AABB(surrounding(*leftBox, *rightBox));
        bboxPtr->min = bounding->min;
        bboxPtr->max = bounding->max;

        nodeIdx++;
    }
}

void initBoundingBoxes(BVH* bvh, GeometricPrimitive* prim) {

    // Allocate memory for bounding boxes
    int nodeIdx = 0;
    bvh->bbox = std::vector<AABB>(bvh->size);
    computeBoundingBoxes_BVH(bvh, prim, nodeIdx);
}


void getBoundingBoxes(BVH* bvh, std::vector<int>& path, std::vector<AABB>& aabb, std::vector<int>& depth) {

    for (int k = 0; k < path.size(); k++) {
        aabb.push_back(bvh->bbox[path[k]]);
        depth.push_back(bvh->node[path[k]].depth);
    }
}


void buildBVHs(BVH& accelerationStructure, GeometricPrimitive* primitive) {

    accelerationStructure = BVH(primitive);
    initBoundingBoxes(&accelerationStructure, primitive);

    int nodeIdx = 0;
    computeDepth(&accelerationStructure, nodeIdx);
}
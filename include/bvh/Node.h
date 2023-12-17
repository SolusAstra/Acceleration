#pragma once
#include "SplitStrategy.h"

class Node {

public:
    int depth = 0;
    int primitiveIdx = -1;
    int branchIdx[2] = { -1, -1 };
    bool isLeaf = false;

public:

    Node() {}
    ~Node() {}

    template <typename T>
    static void generateHierarchy(std::vector<Node>& nodeArray, int& nodeIdx, std::vector<int>& subset, SpatialIndexer& rankIdx);
};

template <typename T>
inline void Node::generateHierarchy(std::vector<Node>& nodeArray, int& nodeIdx, std::vector<int>& subset, SpatialIndexer& rankIdx) {

    const int nP = subset.size();
    Node* currentNode = &nodeArray[nodeIdx];

    // Leaf nodes contain, at most, 1 primitive index (TODO: variable number primitives in leaf node)
    if (nP == 1) {
        currentNode->isLeaf = true;
        currentNode->primitiveIdx = subset[0];
        return;
    }

    // Split remaining set of primitive centroids
    std::vector<int> leftSrtSub, rightSrtSub;
    split<T>(leftSrtSub, rightSrtSub, subset, rankIdx);

    nodeIdx++;
    currentNode->branchIdx[0] = nodeIdx;
    Node::generateHierarchy<T>(nodeArray, nodeIdx, leftSrtSub, rankIdx);

    nodeIdx++;
    currentNode->branchIdx[1] = nodeIdx;
    Node::generateHierarchy<T>(nodeArray, nodeIdx, rightSrtSub, rankIdx);

}
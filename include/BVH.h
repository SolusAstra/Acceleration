#pragma once
#include <iostream>
#include "bvh_util.h"
#include "Particle.h"
#include "Node.h"

template <typename T>
class BVH {

public: 
	std::vector<Node<T>> node;
	std::vector<AABB<T>> bbox;
	Primitive<T> prim;
	size_t size = 0;
	int depth = 0;


	BVH() {}
	~BVH() {}

	// Kick off recursive build
	void recursiveBuild(const Primitive<T>& prim, Idx& rankIdx);

	// Constructor
	BVH(const Primitive<T>& prim, Idx& rankIdx);

	void initBoundingBoxes(const Primitive<T>& prim);
	void computeBoundingBoxes_BVH(const Primitive<T>& prim, int& nodeIdx);

	void computeBoundingBoxes(const Primitive<T>& prim, int& nodeIdx);
	void computeMajorAxis(std::vector<int> subset, int& majorAxis);
	void computeDepth(const Primitive<T>& prim, int& nodeIdx);

};

template <typename T>
void BVH<T>::recursiveBuild(const Primitive<T>& prim, Idx& rankIdx) {

	int rootIdx = 0;
	std::vector<int> idx = prim.idx;
	Node<T>::generateHierarchy(this->node, rootIdx, idx, rankIdx);
}

template <typename T>
BVH<T>::BVH(const Primitive<T>& prim, Idx& rankIdx) {
	this->prim = prim;
	this->size = 2 * prim.N - 1;
	this->node = std::vector<Node<T>>(size);
	recursiveBuild(prim, rankIdx);
}



template <typename T>
void BVH<T>::computeMajorAxis(std::vector<int> subset, int& majorAxis) {
	AABB<T> box = surrounding(prim.vertex, subset);
	return longestAxis(box);
}

template <typename T>
void BVH<T>::initBoundingBoxes(const Primitive<T>& prim) {

	// Allocate memory for bounding boxes
	this->bbox = std::vector<AABB<T>>(this->size);




	int nodeIdx = 0;
	computeBoundingBoxes_BVH(prim, nodeIdx);
	computeDepth(prim, nodeIdx);
}

template <typename T>
void BVH<T>::computeBoundingBoxes_BVH(const Primitive<T>& prim, int& nodeIdx) {

	Node<T>* nodePtr = &this->node[nodeIdx];
	AABB<T>* bboxPtr = &this->bbox[nodeIdx];

	if (nodePtr->isLeaf) {
		int primIdx = nodePtr->primitiveIdx;

		// Set bboxPtr
		bboxPtr->min = prim.vertex[primIdx];
		bboxPtr->max = prim.vertex[primIdx];
		


		//AABB<T>* bounding = new AABB<T>(surrounding(prim.vertex[primIdx]));
		//nodePtr->box = new AABB<T>(surrounding(prim.vertex[primIdx]));
		nodeIdx++;
	}
	else {
		// Construct bounding boxes for children
		for (int i = 0; i < 2; i++) {
			int branchIdx = nodePtr->branchIdx[i];
			computeBoundingBoxes_BVH(prim, branchIdx);
		}

		// Construct bounding box for parent
		int branchIdx0 = nodePtr->branchIdx[0];
		int branchIdx1 = nodePtr->branchIdx[1];
		
		// Get bounding boxes of children
		AABB<T>* leftBox = &this->bbox[branchIdx0];
		AABB<T>* rightBox = &this->bbox[branchIdx1];
		AABB<T>* bounding = new AABB<T>(surrounding(*leftBox, *rightBox));
		bboxPtr->min = bounding->min;
		bboxPtr->max = bounding->max;

		//AABB<T>* leftBox = this->node[branchIdx0].box;
		//AABB<T>* rightBox = this->node[branchIdx1].box;
		//AABB<T>* bounding = new AABB<T>(surrounding(*leftBox, *rightBox));
		//nodePtr->box = bounding;
		nodeIdx++;
	}
}

template <typename T>
void BVH<T>::computeBoundingBoxes(const Primitive<T>& prim, int& nodeIdx) {

	Node<T>* nodePtr = &this->node[nodeIdx];

	if( nodePtr->isLeaf ) {
		int primIdx = nodePtr->primitiveIdx;
		//AABB<T>* bounding = new AABB<T>(surrounding(prim.vertex[primIdx]));
		nodePtr->box = new AABB<T>(surrounding(prim.vertex[primIdx]));
		nodeIdx++;
	} else {
		// Construct bounding boxes for children
		for( int i = 0; i < 2; i++ ) {
			int branchIdx = nodePtr->branchIdx[i];
			computeBoundingBoxes(prim, branchIdx);
		}

		// Construct bounding box for parent
		int branchIdx0 = nodePtr->branchIdx[0];
		int branchIdx1 = nodePtr->branchIdx[1];

		AABB<T>* leftBox = this->node[branchIdx0].box;
		AABB<T>* rightBox = this->node[branchIdx1].box;
		AABB<T>* bounding = new AABB<T>(surrounding(*leftBox, *rightBox));
		nodePtr->box = bounding;
		nodeIdx++;
	}
}

template <typename T>
void BVH<T>::computeDepth(const Primitive<T>& prim, int& nodeIdx) {

	// Recursively dive into tree structure and set depth of each node.
	// Compute final depth of tree structure.
	int bvhDepth = 0;
	Node<T>* nodePtr = &this->node[nodeIdx];

	if (nodePtr->isLeaf) {
		nodePtr->depth = 0;
		bvhDepth = 0;
	}
	else {
		int leftBranch = nodePtr->branchIdx[0];
		int rightBranch = nodePtr->branchIdx[1];
		computeDepth(prim, leftBranch);
		computeDepth(prim, rightBranch);

		// Set depth of current node to 1 + max(left, right)
		nodePtr->depth = 1 + std::max(this->node[leftBranch].depth, this->node[rightBranch].depth);

		bvhDepth = nodePtr->depth;
	}

	// Set depth of BVH to be maximum depth of tree structure
	this->depth = std::max(this->depth, bvhDepth);
}


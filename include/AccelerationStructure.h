#pragma once
#include "BVH.h"
#include "BruteForce.h"
//#include "Primitive.h"
//#include "Node.h"
//#include "AABB.h"

enum ACCEL_TYPE {
	ACCEL_NONE,
	ACCEL_BVH,
	ACCEL_KD_TREE
};

enum ACCEL_FORMAT {
	FLOAT2,
	FLOAT3
};

//
//struct Payload {
//
//    //std::vector<int> path; 
//    int primitiveID;
//    float t;
//    bool wasHit = false;
//
//};
//
//class Environment {
//
//public:
//    std::vector<Payload> payload;
//
//public:
//
//    template <typename T>
//    inline static bool hit(T* accel, Primitive* primitive, const Trace::Ray& ray, Payload& payload) {
//        return false;
//    }
//
//    inline static bool nodeIntersection(AABB* bboxPtr, const Trace::Ray& ray, int closestHit) {
//        bool hitBox = bboxPtr->hit(ray, 0, closestHit);
//        return hitBox;
//    }
//
//    template <>
//    inline static bool hit(AccelStruct::BVH* bvh, Primitive* primitive, const Trace::Ray& ray, Payload& payload) {
//
//        int stack[64]; // Fixed-size stack for traversal
//        int stackIndex = 0;
//        stack[stackIndex++] = 0; // Start with the root node
//
//        bool hitSomething = false;
//        float closestHit = FLT_MAX; // Initialize with maximum float value
//
//        AABB* bboxPtr = &bvh->bbox[0];
//        if (!bboxPtr->hit(ray, 0, closestHit)) {
//            return false;
//        }
//
//        int Niter = 0;
//       do
//       {
//            int nodeIdx = stack[--stackIndex]; // Pop a node index from the stack
//
//
//            Node* nodePtr = &bvh->node[nodeIdx];
//            
//
//            //bool hitNode = Environment::nodeIntersection(&bvh->bbox[nodeIdx], ray, closestHit);
//            //if (!hitNode) {
//            //    continue; // Skip if ray does not hit the bounding box or if box is further than current closest
//            //}
//
//            if (nodePtr->isLeaf) {
//                int primIdx = nodePtr->primitiveIdx;
//
//                float root = -1.0f;
//                primitive->hit(primIdx, ray, closestHit, root);
//
//                //float dist = Triangle::hit(ray, closestSoFar, tri.vertex.data());
//                if (root > 0 && root < closestHit) {
//                    hitSomething = true;
//                    closestHit = root; // Update closest hit distance
//                    payload.wasHit = true;
//                    payload.primitiveID = primIdx; // Record the index of the closest primitive
//                    payload.t = root;
//                    //payload.path.push_back(nodeIdx); // Optionally record the path
//                }
//            }
//            else {
//
//                int L_idx = nodePtr->branchIdx[0];
//                int R_idx = nodePtr->branchIdx[1];
//
//                AABB* L_bboxPtr = &bvh->bbox[L_idx];
//                AABB* R_bboxPtr = &bvh->bbox[R_idx];
//
//                bool hitLeft = L_bboxPtr->hit(ray, 0, closestHit);
//                bool hitRight = R_bboxPtr->hit(ray, 0, closestHit);
//
//                if (hitLeft) {
//                    stack[stackIndex++] = L_idx;
//                }
//
//                if (hitRight) {
//                    stack[stackIndex++] = R_idx;
//                }
//
//                //// Push child nodes onto the stack
//                //stack[stackIndex++] = nodePtr->branchIdx[0]; // Left child
//                //stack[stackIndex++] = nodePtr->branchIdx[1]; // Right child
//            }
//
//            Niter++;
//       } while (stackIndex > 0);
//
//        return hitSomething;
//    }
//
//    template <>
//    inline static bool hit(AccelStruct::dBVH* bvh, Primitive* primitive, const Trace::Ray& ray, Payload& payload) {
//
//        int stack[64]; // Fixed-size stack for traversal
//        int stackIndex = 0;
//        stack[stackIndex++] = 0; // Start with the root node
//
//        bool hitSomething = false;
//        float closestHit = FLT_MAX; // Initialize with maximum float value
//
//        while (stackIndex > 0) {
//            int nodeIdx = stack[--stackIndex]; // Pop a node index from the stack
//
//
//            Node* nodePtr = &bvh->nodePtr[nodeIdx];
//            AABB* bboxPtr = &bvh->bboxPtr[nodeIdx];
//            //Node* nodePtr = &bvh->node[nodeIdx];
//            //AABB* bboxPtr = &bvh->bbox[nodeIdx];
//
//
//            if (!bboxPtr->hit(ray, 0, closestHit)) {
//                continue; // Skip if ray does not hit the bounding box or if box is further than current closest
//            }
//
//            if (nodePtr->isLeaf) {
//                int primIdx = nodePtr->primitiveIdx;
//
//                float root = -1.0f;
//                primitive->hit(primIdx, ray, closestHit, root);
//
//                //float dist = Triangle::hit(ray, closestSoFar, tri.vertex.data());
//                if (root > 0 && root < closestHit) {
//                    hitSomething = true;
//                    closestHit = root; // Update closest hit distance
//
//                    payload.wasHit = true;
//                    payload.primitiveID = primIdx; // Record the index of the closest primitive
//                    payload.t = root;
//                    //payload.path.push_back(nodeIdx); // Optionally record the path
//                }
//            }
//            else {
//                // Push child nodes onto the stack
//                stack[stackIndex++] = nodePtr->branchIdx[0]; // Left child
//                stack[stackIndex++] = nodePtr->branchIdx[1]; // Right child
//            }
//        }
//
//        return hitSomething;
//    }
//
//    template <>
//    inline static bool hit(AccelStruct::BruteForce* linearSearch, Primitive* primitive, const Trace::Ray& ray, Payload& payload) {
//
//
//
//        int objectIndex = -1;
//        bool hitDetected = false;
//        float closestHit = FLT_MAX;
//
//        std::vector<int> objHits(0);
//        int totalPrimitives = primitive->N;
//
//        for (int k = 0; k < totalPrimitives; k++) {
//
//            float root = -1.0f;
//            bool primHit = primitive->hit(k, ray, closestHit, root);
//            
//
//            if (root > 0 && root < closestHit) {
//
//                hitDetected = true;
//                closestHit = root;
//                objectIndex = k;
//                objHits.push_back(k);
//
//                payload.wasHit = true;
//                payload.primitiveID = k; // Record the index of the closest primitive
//                payload.t = root;
//            }
//
//        }
//
//        return hitDetected;
//    }
//
//};



#pragma once
#include "Particle.h"


class dTriangle : public dPrimitive {
public:
    dTriangle(const std::vector<float3>& vtx, const std::vector<int3>& fcs) {
        type = ACCEL_INPUT_TYPE::TRIANGLE;
        N = vtx.size();
        F = fcs.size();

        vertex = new float3[N];
        std::copy(vtx.begin(), vtx.end(), vertex);

        face = new int3[F];
        std::copy(fcs.begin(), fcs.end(), face);
    }

    virtual ~dTriangle() {
        delete[] vertex; // Free memory allocated for vertices
        delete[] face;   // Free memory allocated for faces
    }

    virtual bool hit(int idx, const Trace::Ray& ray, float t_max, float& root) const override {
        float3 vertexA = vertex[face[idx].x];
        float3 vertexB = vertex[face[idx].y];
        float3 vertexC = vertex[face[idx].z];

        float3 edge1 = vertexB - vertexA;
        float3 edge2 = vertexC - vertexA;
        float3 h = cross(ray.dir, edge2);
        float a = dot(edge1, h);

        if (a > -0.00001f && a < 0.00001f) {
            return false;
        }

        float f = 1.0f / a;
        float3 s = ray.org - vertexA;
        float u = f * dot(s, h);
        if (u < 0.0f || u > 1.0f) {
            return false;
        }

        float3 q = cross(s, edge1);
        float v = f * dot(ray.dir, q);
        if (v < 0.0f || u + v > 1.0f) {
            return false;
        }

        root = f * dot(edge2, q);
        if (root > 0.0001f && root < t_max) {
            return true;
        }

        return false;
    }
};

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

class Triangle : public Primitive {

public:

    // Constructors
    Triangle() {
        this->type = ACCEL_INPUT_TYPE::TRIANGLE;
        this->vertex = std::vector<float3>();
        this->N = 0;

    }

    Triangle(std::vector<float3>& vtx, std::vector<int3>& idx) {
        this->type = ACCEL_INPUT_TYPE::TRIANGLE;
        this->vertex = vtx;
        this->N = idx.size();
        this->face = idx;
    }

    __host__ __device__ void getTriangleVertices(int objID, float3& A, float3& B, float3& C) const {
        int3 triIndices = face[objID];
        A = vertex[triIndices.x];
        B = vertex[triIndices.y];
        C = vertex[triIndices.z];
    }

    virtual Primitive* reduceToPrimitive() const override {

        int nFaces = face.size();
        std::vector<float3> barycenter(nFaces);

        for (int k = 0; k < nFaces; k++) {
            barycenter[k] = computeBarycenter(vertex, face, k);
        }

        Particle particlePrimtive = Particle(barycenter);
        return &particlePrimtive;

    }

    virtual bool hit(int idx, const Trace::Ray& ray, float t_max, float& root) const override {

        float3 vertexA = vertex[face[idx].x];
        float3 vertexB = vertex[face[idx].y];
        float3 vertexC = vertex[face[idx].z];

        float3 edge1 = vertexB - vertexA;
        float3 edge2 = vertexC - vertexA;
        float3 h = cross(ray.dir, edge2);
        float a = dot(edge1, h);

        if (a > -0.00001f && a < 0.00001f) {
            return false;
        }

        float f = 1.0f / a;
        float3 s = ray.org - vertexA;
        float u = f * dot(s, h);
        if (u < 0.0f || u > 1.0f) {
            return false;
        }

        float3 q = cross(s, edge1);
        float v = f * dot(ray.dir, q);
        if (v < 0.0f || u + v > 1.0f) {
            return false;
        }

        root = f * dot(edge2, q);
        if (root > 0.0001f && root < t_max) {
            return true;
        }

        return false;
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

    virtual dPrimitive* createDeviceVersion() const override {
        // Create a device version of dParticle
        // This implementation assumes the existence of a CPU counterpart
        // Replace with actual logic to copy data from the CPU version

        return new dTriangle(vertex, face);
    }

};
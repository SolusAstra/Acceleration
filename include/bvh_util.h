#pragma once
#include "sutil\vec_math.h"
#include <algorithm>
#include <vector>
#include <numeric>
#include <functional>

#include "Primitive.h"
#include "AABB.h"





template <typename T>
void sortAlongX(const Primitive<T>& p, std::vector<int>& indices)
{
	std::sort(indices.begin(), indices.end(), [&p](int a, int b) {
		return p.vertex[a].x < p.vertex[b].x;
		});
}

template <typename T>
void sortAlongY(const Primitive<T>& p, std::vector<int>& indices)
{
	std::sort(indices.begin(), indices.end(), [&p](int a, int b) {
		return p.vertex[a].y < p.vertex[b].y;
		});
}

void sortAlongZ(const Primitive<float3>& p, std::vector<int>& srtIdx)
{
	std::sort(srtIdx.begin(), srtIdx.end(), [&p](int a, int b) {
		return p.vertex[a].z < p.vertex[b].z;
		});
}


struct Idx {
	std::vector<std::vector<int>> data;
	//std::vector<std::vector<int>> sortedData;
	const int N;
	const int M;

	Idx(int N, int M) : N(N), M(M) {
		data.resize(N);
		for (int i = 0; i < N; i++) {
			data[i].resize(M);
			// Initialize data
			std::iota(data[i].begin(), data[i].end(), 0);
		}
	}

	std::vector<int>& operator()(int axis) { return data[axis]; }
	int& operator()(int axis, int element) { return data[axis][element]; }

	template <typename T>
	void rank(const Primitive<T>& p);

	template<typename T>
	void sort(const Primitive<T>& p) {}
	template<>
	void sort<float2>(const Primitive<float2>& p);
	template<>
	void sort<float3>(const Primitive<float3>& p);

};





template<typename T>
inline void Idx::rank(const Primitive<T>& p) {

	// Sort pitives
	sort(p);

	// Temporarily copy 
	Idx temp(*this);
	for (int i = 0; i < M; ++i) {
		temp(0, this->operator()(0, i)) = i;
		temp(1, this->operator()(1, i)) = i;
	}

	// Save ranked indices
	this->data.swap(temp.data);
}

template<>
void Idx::sort<float2>(const Primitive<float2>& p) {
	sortAlongX(p, this->data[0]);
	sortAlongY(p, this->data[1]);
}

template<>
void Idx::sort<float3>(const Primitive<float3>& p) {
	sortAlongX(p, this->data[0]);
	sortAlongY(p, this->data[1]);
	sortAlongZ(p, this->data[2]);
}

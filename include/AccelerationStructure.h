#pragma once

enum ACCEL_TYPE {
	ACCELL_NONE,
	ACCELL_BVH,
	ACCELL_KD_TREE
};

enum ACCEL_FORMAT {
	FLOAT2,
	FLOAT3
};

namespace ACCEL {

	struct AccelerationStructure {
		int type;
	};

	struct BVH : AccelerationStructure {
		int type = ACCELL_BVH;
	};

	struct KD_TREE : AccelerationStructure {
		int type = ACCELL_KD_TREE;
	};

};
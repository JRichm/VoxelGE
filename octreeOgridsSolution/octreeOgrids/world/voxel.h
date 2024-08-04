#pragma once
struct Voxel {
	int type;
	// init empty voxel
	Voxel() : type(0) {} 

	//int voxel with type
	Voxel(int t) : type(t) {}
};


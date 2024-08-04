#pragma once
class home {

};

// octree of grids riscadoa.com/game-dev/voxel-engine-1

// each node in the octree represents a 32x32x32 chunk of the map, represented by a voxel grid
// which can be further divided into 8 smaller nodes when you get closer	< idk if i wan this, and instead use 64x64x64 chunks of pure voxels

// the world is built on an octree of grids/chunks,
// should each chunk keep a reference to its neighboring chunks?


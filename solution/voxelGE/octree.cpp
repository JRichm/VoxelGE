#include <iostream>
#include "Octree.h"
#include <vector>
#include <memory>


void Octree::subdivide(OctreeNode* node) {
    Point3D min = node->boundary.min;
    Point3D max = node->boundary.max;
    Point3D mid(
        (min.position.x + max.position.x) / 2,
        (min.position.y + max.position.y) / 2,
        (min.position.z + max.position.z) / 2
    );

    node->children[0] = std::make_unique<OctreeNode>(Box(min, mid));
    node->children[1] = std::make_unique<OctreeNode>(Box(Point3D(mid.position.x, min.position.y, min.position.z), Point3D(max.position.x, mid.position.y, mid.position.z)));
    node->children[2] = std::make_unique<OctreeNode>(Box(Point3D(min.position.x, mid.position.y, min.position.z), Point3D(mid.position.x, max.position.y, mid.position.z)));
    node->children[3] = std::make_unique<OctreeNode>(Box(Point3D(mid.position.x, mid.position.y, min.position.z), Point3D(max.position.x, max.position.y, mid.position.z)));
    node->children[4] = std::make_unique<OctreeNode>(Box(Point3D(min.position.x, min.position.y, mid.position.z), Point3D(mid.position.x, mid.position.y, max.position.z)));
    node->children[5] = std::make_unique<OctreeNode>(Box(Point3D(mid.position.x, min.position.y, mid.position.z), Point3D(max.position.x, mid.position.y, max.position.z)));
    node->children[6] = std::make_unique<OctreeNode>(Box(Point3D(min.position.x, mid.position.y, mid.position.z), Point3D(mid.position.x, max.position.y, max.position.z)));
    node->children[7] = std::make_unique<OctreeNode>(Box(mid, max));

	
}

bool Octree::insert(OctreeNode* node, const Voxel& voxel, int depth) {

    if (!node->boundary.contains(Point3D(voxel.position.x, voxel.position.y, voxel.position.z))) {
        return false;
    }

    if (depth == maxDepth) {
        node->voxels.push_back(voxel);
        node->hasVoxels = true;
        return true;
    }

    if (node->children[0] == nullptr) {
        subdivide(node);
    }

    for (auto& child : node->children) {
        if (insert(child.get(), voxel, depth + 1)) {
            return true;
        }
    }

    return false;
}

std::vector<Voxel> Octree::getVoxelsInRange(const Box& range) {
    std::vector<Voxel> result;
    collectVoxelsInRange(root.get(), range, result);
    return result;
}

std::vector<Voxel> Octree::getVoxelsForRendering() const {
    std::vector<Voxel> result;
    collectVoxels(root.get(), result);

    return result;
}

void Octree::collectVoxels(OctreeNode* node, std::vector<Voxel>& result) const {
    if (node == nullptr) {
        return;
    }

    result.insert(result.end(), node->voxels.begin(), node->voxels.end());

    for (const auto& child : node->children) {
        collectVoxels(child.get(), result);
    }
}

void Octree::collectVoxelsInRange(OctreeNode* node, const Box& range, std::vector<Voxel>& result) {
    if (!node->boundary.intersects(range))
        return;

    if (node->hasVoxels) {
        for (const auto& voxel : node->voxels) {
            if (range.contains(Point3D(voxel.position.x, voxel.position.y, voxel.position.z))) {
                result.push_back(voxel);
            }
        }
    }

    for (auto& child : node->children) {
        if (child != nullptr) {
            collectVoxelsInRange(child.get(), range, result);
        }
    }
}

const float Voxel::vertices[108] = {

    // front
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    // back
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    // left
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    // right
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     
     // bottom
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    // top
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};
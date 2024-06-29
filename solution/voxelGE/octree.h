#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>


struct Point3D {
    glm::vec3 position;

    Point3D(float x, float y, float z) : position(x, y, z) {}
};

struct Box {
    Point3D min, max;

    Box(Point3D _min, Point3D _max) : min(_min), max(_max) {}

    bool contains(const Point3D& point) const {
        return (
            point.position.x >= min.position.x && point.position.x <= max.position.x &&
            point.position.y >= min.position.y && point.position.y <= max.position.y &&
            point.position.z >= min.position.z && point.position.z <= max.position.z
            );
    }

    bool intersects(const Box& other) const {
        return !(
            other.min.position.x > max.position.x || other.max.position.x < min.position.x ||
            other.min.position.y > max.position.y || other.max.position.y < min.position.y ||
            other.min.position.z > max.position.z || other.max.position.z < min.position.z
            );
    }
};

struct Voxel {
    glm::ivec3 position;
    glm::vec3 color;

    Voxel(glm::vec3 pos, glm::vec3 col) : position(pos), color(col) {}

    static const float vertices[108];
};

class OctreeNode {
public:
    Box boundary;
    std::vector<Voxel> voxels;
    bool hasVoxels;
    std::vector<std::unique_ptr<OctreeNode>> children;

    OctreeNode(const Box& _boundary) : boundary(_boundary), hasVoxels(false), children(8) {}
};

class Octree {
public:
    Octree(const Box& boundary, int _maxDepth) : root(std::make_unique<OctreeNode>(boundary)), maxDepth(_maxDepth) {}

    bool insert(const Voxel& voxel) {
        return insert(root.get(), voxel, 0);
    }

    std::vector<Voxel> getVoxelsInRange(const Box& range);
    std::vector<Voxel> getVoxelsForRendering() const;


private:
    std::unique_ptr<OctreeNode> root;
    int maxDepth;

    void subdivide(OctreeNode* node);
    bool insert(OctreeNode* node, const Voxel& voxel, int depth);
    void collectVoxelsInRange(OctreeNode* node, const Box& range, std::vector<Voxel>& result);
    void collectVoxels(OctreeNode* node, std::vector<Voxel>& result) const;

};


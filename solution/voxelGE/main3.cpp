#include <iostream>
#include "Octree.h"

int main() {
    Box boundary(Point3D(0, 0, 0), Point3D(100, 100, 100));
    Octree octree(boundary, 4);

    Voxel voxel1(glm::ivec3(10, 10, 10), glm::vec3(1.0f, 0.0f, 0.0f));
    Voxel voxel2(glm::ivec3(20, 20, 20), glm::vec3(0.0f, 1.0f, 0.0f));
    Voxel voxel3(glm::ivec3(30, 30, 30), glm::vec3(0.0f, 0.0f, 1.0f));

    octree.insert(voxel1);
    octree.insert(voxel2);
    octree.insert(voxel3);

    std::cout << "Voxels inserted successfully." << std::endl;


    Box range(Point3D(5, 5, 5), Point3D(25, 25, 25));

    std::vector<Voxel> voxelsInRange = octree.getVoxelsInRange(range);

    std::cout << "Voxels in range:" << std::endl;
    for (const auto& voxel : voxelsInRange) {
        std::cout << "Position: (" << voxel.position.x << ", " << voxel.position.y << ", " << voxel.position.z << "), Color: (" << voxel.color.x << ", " << voxel.color.y << ", " << voxel.color.z << ")" << std::endl;
    }


    return 0;
}
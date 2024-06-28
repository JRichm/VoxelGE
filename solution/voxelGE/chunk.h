#pragma once

#include <vector>
#include <unordered_map>

#include "voxel.h"

class Chunk {
public:
    Chunk();
    ~Chunk();

    void setVoxel(const glm::vec3& position, const Voxel& voxel);
    Voxel getVoxel(const glm::vec3& position) const;


private:
    std::unordered_map<glm::ivec3, Voxel> voxels;
}

//     static constexpr int CHUNK_SIZE = 16;
// 
//     Voxel voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
// 
//     Chunk() {
//         // Initialize all voxels in the chunk
//         for (int x = 0; x < CHUNK_SIZE; ++x) {
//             for (int y = 0; y < CHUNK_SIZE; ++y) {
//                 for (int z = 0; z < CHUNK_SIZE; ++z) {
//                     // Example: Solid voxels at the bottom of the chunk
//                     if (y < CHUNK_SIZE / 2) {
//                         voxels[x][y][z].type = Voxel::Type::Solid;
//                         voxels[x][y][z].color = glm::vec3(0.5f, 0.5f, 0.5f);
//                     }
//                     else {
//                         voxels[x][y][z].type = Voxel::Type::Air;
//                         voxels[x][y][z].color = glm::vec3(1.0f);
//                     }
//                 }
//             }
//         }
//     };
// 
//     //  Voxel& getVoxel(int x, int y, int z);
//     //  const Voxel& getVoxel(int x, int y, int z) const;
// 
// private:
// };
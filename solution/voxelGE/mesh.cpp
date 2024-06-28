#include "mesh.h"
#include "chunk.h"

void Mesh::clear() {
    vertices.clear();
    indices.clear();
}


// Mesh generateMesh(const Chunk& chunk) {
//     Mesh mesh;
// 
//     float voxelSize = 0.05f; // Adjust according to your voxel size
// 
//     for (int x = 0; x < Chunk::CHUNK_SIZE; ++x) {
//         for (int y = 0; y < Chunk::CHUNK_SIZE; ++y) {
//             for (int z = 0; z < Chunk::CHUNK_SIZE; ++z) {
//                 Voxel voxel = chunk.getVoxel(x, y, z);
//                 voxel.setType(Voxel::Type::Solid);
//                 if (voxel.getType() == Voxel::Type::Solid) {
//                     // Calculate positions based on voxel position and size
//                     glm::vec3 bottomLeftFront = glm::vec3(x * voxelSize, y * voxelSize, z * voxelSize);
//                     glm::vec3 bottomRightFront = glm::vec3((x + 1) * voxelSize, y * voxelSize, z * voxelSize);
//                     glm::vec3 topRightFront = glm::vec3((x + 1) * voxelSize, (y + 1) * voxelSize, z * voxelSize);
//                     glm::vec3 topLeftFront = glm::vec3(x * voxelSize, (y + 1) * voxelSize, z * voxelSize);
//                     glm::vec3 bottomLeftBack = glm::vec3(x * voxelSize, y * voxelSize, (z + 1) * voxelSize);
//                     glm::vec3 bottomRightBack = glm::vec3((x + 1) * voxelSize, y * voxelSize, (z + 1) * voxelSize);
//                     glm::vec3 topRightBack = glm::vec3((x + 1) * voxelSize, (y + 1) * voxelSize, (z + 1) * voxelSize);
//                     glm::vec3 topLeftBack = glm::vec3(x * voxelSize, (y + 1) * voxelSize, (z + 1) * voxelSize);
// 
//                     // Front face vertices
//                     mesh.vertices.push_back({ bottomLeftFront });
//                     mesh.vertices.push_back({ bottomRightFront });
//                     mesh.vertices.push_back({ topRightFront });
//                     mesh.vertices.push_back({ topLeftFront });
// 
//                     // Back face vertices
//                     mesh.vertices.push_back({ bottomRightBack });
//                     mesh.vertices.push_back({ bottomLeftBack });
//                     mesh.vertices.push_back({ topLeftBack });
//                     mesh.vertices.push_back({ topRightBack });
// 
//                     // Indices for the front face (two triangles)
//                     unsigned int baseIndex = mesh.vertices.size() - 8; // Starting index of the last added vertices
//                     mesh.indices.push_back(baseIndex);
//                     mesh.indices.push_back(baseIndex + 1);
//                     mesh.indices.push_back(baseIndex + 2);
//                     mesh.indices.push_back(baseIndex);
//                     mesh.indices.push_back(baseIndex + 2);
//                     mesh.indices.push_back(baseIndex + 3);
// 
//                     // Indices for the back face (two triangles)
//                     mesh.indices.push_back(baseIndex + 4);
//                     mesh.indices.push_back(baseIndex + 5);
//                     mesh.indices.push_back(baseIndex + 6);
//                     mesh.indices.push_back(baseIndex + 4);
//                     mesh.indices.push_back(baseIndex + 6);
//                     mesh.indices.push_back(baseIndex + 7);
// 
//                     // Left face vertices
//                     mesh.vertices.push_back({ bottomLeftBack });
//                     mesh.vertices.push_back({ bottomLeftFront });
//                     mesh.vertices.push_back({ topLeftFront });
//                     mesh.vertices.push_back({ topLeftBack });
// 
//                     // Indices for the left face (two triangles)
//                     baseIndex = mesh.vertices.size() - 8; // Update base index
//                     mesh.indices.push_back(baseIndex);
//                     mesh.indices.push_back(baseIndex + 1);
//                     mesh.indices.push_back(baseIndex + 2);
//                     mesh.indices.push_back(baseIndex);
//                     mesh.indices.push_back(baseIndex + 2);
//                     mesh.indices.push_back(baseIndex + 3);
// 
//                     // Right face vertices
//                     mesh.vertices.push_back({ bottomRightFront });
//                     mesh.vertices.push_back({ bottomRightBack });
//                     mesh.vertices.push_back({ topRightBack });
//                     mesh.vertices.push_back({ topRightFront });
// 
//                     // Indices for the right face (two triangles)
//                     baseIndex = mesh.vertices.size() - 8; // Update base index
//                     mesh.indices.push_back(baseIndex);
//                     mesh.indices.push_back(baseIndex + 1);
//                     mesh.indices.push_back(baseIndex + 2);
//                     mesh.indices.push_back(baseIndex);
//                     mesh.indices.push_back(baseIndex + 2);
//                     mesh.indices.push_back(baseIndex + 3);
// 
//                     // Top face vertices
//                     mesh.vertices.push_back({ topLeftFront });
//                     mesh.vertices.push_back({ topRightFront });
//                     mesh.vertices.push_back({ topRightBack });
//                     mesh.vertices.push_back({ topLeftBack });
// 
//                     // Indices for the top face (two triangles)
//                     baseIndex = mesh.vertices.size() - 8; // Update base index
//                     mesh.indices.push_back(baseIndex);
//                     mesh.indices.push_back(baseIndex + 1);
//                     mesh.indices.push_back(baseIndex + 2);
//                     mesh.indices.push_back(baseIndex);
//                     mesh.indices.push_back(baseIndex + 2);
//                     mesh.indices.push_back(baseIndex + 3);
// 
//                     // Bottom face vertices
//                     mesh.vertices.push_back({ bottomLeftBack });
//                     mesh.vertices.push_back({ bottomRightBack });
//                     mesh.vertices.push_back({ bottomRightFront });
//                     mesh.vertices.push_back({ bottomLeftFront });
// 
//                     // Indices for the bottom face (two triangles)
//                     baseIndex = mesh.vertices.size() - 8; // Update base index
//                     mesh.indices.push_back(baseIndex);
//                     mesh.indices.push_back(baseIndex + 1);
//                     mesh.indices.push_back(baseIndex + 2);
//                     mesh.indices.push_back(baseIndex);
//                     mesh.indices.push_back(baseIndex + 2);
//                     mesh.indices.push_back(baseIndex + 3);
//                 }
//             }
//         }
//     }
// 
//     return mesh;
// }


#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "chunk.h"

struct Vertex {
    glm::vec3 Position;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    void clear();
};

Mesh generateMesh(const Chunk& chunk);
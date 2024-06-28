#pragma once

#include <glm/glm.hpp>

class Voxel {
public:
    enum class Type { Air, Solid };

    Voxel(Type type = Type::Air, const glm::vec3& position = glm::vec3(0.0f));

    Type getType() const;
    void setType(Type newType);

    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3& newPosition);

    Type type;
    glm::vec3 color;

private:
    glm::vec3 position;
};
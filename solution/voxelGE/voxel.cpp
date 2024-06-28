#include "Voxel.h"

Voxel::Voxel(Type type, const glm::vec3& position) : type(type), position(position) {}


Voxel::Type Voxel::getType() const {
    return type;
}

void Voxel::setType(Type newType) {
    type = newType;
}

glm::vec3 Voxel::getPosition() const {
    return position;
}

void Voxel::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}
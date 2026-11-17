#pragma once

#include <glm/glm.hpp>

struct Camera3D {
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    
    float yaw;
    float pitch;
    float roll;

};
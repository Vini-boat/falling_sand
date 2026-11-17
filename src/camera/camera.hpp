#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Camera3D {
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    
    float yaw;
    float pitch;
    float roll;

    glm::vec3 getRight();

};
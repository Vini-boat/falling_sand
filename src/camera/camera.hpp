#pragma once

#include <glm/glm.hpp>


struct Camera3D {
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    
    float yaw         = -90.0f;
    float pitch       = 0.0f; 
    float roll        = 0.0f; 
    float pitch_limit = 89.0f;


    float speed       = 2.5f;
    float sensitivty  = 0.1f;
    float fov         = 45.0f;

    float aspect_ratio;
    float z_near      = 0.1f;
    float z_far       = 100.0f;

    enum Direction {
        FRONT,
        BACK,
        LEFT,
        RIGHT
    };

    Camera3D(
        glm::vec3 pos, 
        glm::vec3 front, 
        glm::vec3 up,
        float aspect
    );

    void move(Direction dir, float dt);

    void rotate(glm::vec2 screen_offset);

    glm::mat4 getView();

    glm::mat4 getProjection();

    private:
        void recalculateVectors();
};
#include "camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera3D::Camera3D(
    glm::vec3 pos, 
    glm::vec3 front, 
    glm::vec3 up,
    float aspect
){
    this->position     = pos;
    this->front        = front;
    this->up           = up;
    this->aspect_ratio = aspect;
    recalculateVectors();
}

void Camera3D::recalculateVectors(){
    right        = glm::normalize(glm::cross(front, up));
}

void Camera3D::move(Camera3D::Direction dir, float dt){
    switch (dir) {
        case FRONT: position -= front * speed * dt ; break;
        case BACK:  position += front * speed * dt ; break;
        case LEFT:  position += right * speed * dt ; break;
        case RIGHT: position -= right * speed * dt ; break;
    };
}

void Camera3D::rotate(glm::vec2 screen_offset){
    auto offset = screen_offset * sensitivty;

    yaw  += offset.x;
    pitch = glm::clamp(pitch + offset.y, -pitch_limit, pitch_limit);
    
    glm::vec3 dir;

    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(dir);
    recalculateVectors();
}

glm::mat4 Camera3D::getView(){
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera3D::getProjection(){
    return glm::perspective(fov, aspect_ratio, z_near, z_far);
}
#include "camera_node.hpp"

Camera_node::Camera_node():
    Node(),
    projectionMatrix{}{}

Camera_node::Camera_node(glm::mat4 matrix):
    Node(),
    projectionMatrix{matrix}{}

void Camera_node::setEnable(bool enable){
    isEnabled = enable;
}
glm::mat4 Camera_node::getProjectionMatrix(){
    return projectionMatrix;
}
void Camera_node::setProjectionMatrix(glm::mat4 matrix){
    projectionMatrix = matrix;

}
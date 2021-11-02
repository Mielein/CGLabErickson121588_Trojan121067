#include "camera_node.hpp"

Camera_node::Camera_node():
    Node(),
    projectionMatrix{}{}

Camera_node::Camera_node(std::string _name):
    Node{_name}{}

bool Camera_node::getEnabled(){
    return isEnabled;
}
void Camera_node::setEnable(bool enable){
    isEnabled = enable;
}
glm::mat4 Camera_node::getProjectionMatrix(){
    return projectionMatrix;
}
void Camera_node::setProjectionMatrix(glm::mat4 matrix){
    projectionMatrix = matrix;

} 
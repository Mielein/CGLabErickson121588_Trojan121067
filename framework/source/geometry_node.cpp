#include "geometry_node.hpp"

Geometry_node::Geometry_node():
    Node(),
    geometry{}{}

Geometry_node::Geometry_node(std::string _name):
    Node{_name},
    geometry{}{}

Geometry_node::Geometry_node(std::string _name, glm::fmat4 _localTransform):
    Node{_name, _localTransform}{}

Geometry_node::Geometry_node(std::string _name, std::shared_ptr<Node> _parent, glm::fmat4 _localTransform):
    Node{_name,_parent, _localTransform}{}


model Geometry_node::getGeometry(){
    return geometry;
}

void Geometry_node::setGeometry(model g){
    geometry = g;
}
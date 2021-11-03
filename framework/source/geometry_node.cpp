#include "geometry_node.hpp"

Geometry_node::Geometry_node():
    Node(),
    geometry{}{}

Geometry_node::Geometry_node(std::string _name):
    Node{_name},
    geometry{}{}

model Geometry_node::getGeometry(){
    return geometry;
}

void Geometry_node::setGeometry(model g){
    geometry = g;
}
#include "point_light_node.hpp"

    Point_light_node::Point_light_node():
        Node(),
        lightIntesity{},
        lightColour{}{}
    Point_light_node::Point_light_node(std::string _name):
        Node(_name),
        lightIntesity{},
        lightColour{}{}
    Point_light_node::Point_light_node(std::string _name, glm::fvec3 lightColour):
        Node( _name),
        lightColour{lightColour},
        lightIntesity{}{} 
    Point_light_node::Point_light_node(std::string _name, glm::fvec3 lightColour, int lightIntesity):
        Node( _name),
        lightColour{lightColour},
        lightIntesity{lightIntesity}{} 
    Point_light_node::Point_light_node(std::string _name, std::shared_ptr<Node> _parent, glm::fvec3 lightColour, int lightIntesity):
        Node( _name),
        lightColour{lightColour},
        lightIntesity{lightIntesity}{} 

    Point_light_node::Point_light_node(std::string _name, glm::fmat4 _localtransform, std::shared_ptr<Node> _parent, glm::fvec3 lightColour, int lightIntesity):
        Node( _name),
        lightColour{lightColour},
        lightIntesity{lightIntesity}{} 
        
    int Point_light_node::getLightIntesity(){
        return lightIntesity;
    }
        
    void Point_light_node::setLightIntesity(int light){
        lightIntesity = light;
    }
    glm::fvec3 Point_light_node::getLightColour(){
        return lightColour;
    }
    void Point_light_node::setLightIntesity(glm::fvec3 colour){
        lightColour = colour;
    }
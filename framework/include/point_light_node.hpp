#ifndef POINT_LIGHT_NODE_HPP;
#define POINT_LIGHT_NODE_HPP;

#include "node.hpp"

class Point_light_node : public Node{
    private:
        int lightIntesity;
        glm::fvec3 lightColour;
    public:
        Point_light_node();
        Point_light_node(std::string _name);
        Point_light_node(std::string _name, glm::fvec3 lightColour);
        Point_light_node(std::string _name, glm::fvec3 lightColour, int lightIntesity);
        Point_light_node(std::string _name, std::shared_ptr<Node> _parent, glm::fvec3 lightColour, int lightIntesity);
        int getLightIntesity();
        void setLightIntesity(int light);
        glm::fvec3 getLightColour();
        void setLightIntesity(glm::fvec3 colour);
};


#endif
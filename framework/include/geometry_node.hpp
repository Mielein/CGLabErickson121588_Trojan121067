#ifndef GEOMETRY_NODE_HPP;
#define GEOMETRY_NODE_HPP;

#include "node.hpp"
#include "model.hpp"

class Geometry_node : public Node{
    private:
        model geometry;
    public:
        Geometry_node();
        Geometry_node(std::string _name);
        Geometry_node(std::string _name, glm::fmat4 _localTransform);
        Geometry_node(std::string _name, std::shared_ptr<Node> _parent, glm::fmat4 _localTransform);
        model getGeometry();
        void setGeometry(model g);
};


#endif
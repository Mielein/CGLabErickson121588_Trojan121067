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
        model getGeometry();
        void setGeometry(model g);
};


#endif
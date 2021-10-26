#ifndef SCENE_GRAPH_HPP;
#define SCENE_GRAPH_HPP;

#include "node.hpp"
#include <string>

class scene_graph{
    private:
        std::string name;
        Node root;
        void setName();
        void setRoot();

    public:
        std::string getName();
        Node getRoot();
        std::string printClass();
};

#endif
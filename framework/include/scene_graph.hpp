#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include "node.hpp"
#include "camera_node.hpp"
#include <string>

class Scene_graph{
    private:
        std::string name;
        Node root;
        void setName(std::string n);
        void setRoot(Node r);

    public:
        Scene_graph();
        Scene_graph(std::string n);
        Scene_graph(std::string n, Node r);
        std::string getName() const;
        Node getRoot()  const;
        std::string printClass() const;
};

#endif
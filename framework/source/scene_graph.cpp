#include "scene_graph.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

Scene_graph::Scene_graph():
    name{" "}{}

Scene_graph::Scene_graph(std::string n){
    setName(n);
} 

Scene_graph::Scene_graph(std::string n, Node r){
    setRoot(r);
    setName(n);
} 
    /* Node root = Node("root");
    setRoot(root); */


std::string Scene_graph::getName() const{
    return name;
}

void Scene_graph::setName(std::string n){
    name = n;
}

Node Scene_graph::getRoot() const{
    return root;
}

void Scene_graph::setRoot(Node r){
    root = r;
}

std::string printChildren(std::vector<std::shared_ptr<Node>> children){
    std::string debug_return;
    for(auto const& x : children){
        std::cout << x->getName() << " -> "; //ToDo (not that much more) the format is wrong but else... ITS DONE
        if(x->getChildrenList().size() != 0){
            printChildren(x->getChildrenList());
        }
        std::cout << std::endl;
    }
    return debug_return;
}

std::string Scene_graph::printClass() const{
    //std::cout<<root->getName()<<"->";
    //std::cout<<root->getName()<<std::endl;
    printChildren(root.getChildrenList());
}

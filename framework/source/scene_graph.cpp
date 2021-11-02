#include "scene_graph.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

Scene_graph::Scene_graph():
    name{" "},
    root{}{}

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

std::string printChildren(std::vector<Node> children){
    for(Node const& x : children){
        std::cout<<x.getParent()->getName()<<" -> "<<x.getName()<<"->";
        printChildren(x.getChildrenList());
        std::cout<<std::endl;
    }
}

std::string Scene_graph::printClass() const{
    //std::cout<<root.getName()<<"->";
    std::cout<<printChildren(root.getChildrenList());
    
}

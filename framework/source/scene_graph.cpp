#include "scene_graph.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

Scene_graph::Scene_graph():
    name{" "},
    root{}{}

Scene_graph::Scene_graph(std::string n, Node r):
    name{n},
    root{r}{}

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
        std::cout<<x.getName()<<", ";
        std::cout<<"->"<<std::endl;
        printChildren(x.getChildrenList());
    }
}

std::string Scene_graph::printClass() const{
    std::cout<<root.getName()<<"->";
    printChildren(root.getChildrenList());
}

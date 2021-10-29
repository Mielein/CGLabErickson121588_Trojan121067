#include "scene_graph.hpp"

Scene_graph::Scene_graph():
    name{},
    root{}{}

Scene_graph::Scene_graph(std::string n, Node r):
    name{n},
    root{r}{}

std::string Scene_graph::getName(){
    return name;
}
void Scene_graph::setName(std::string n){
    name = n;
}

Node Scene_graph::getRoot(){
    return root;
}

void Scene_graph::setRoot(Node r){
    root = r;
}

std::string Scene_graph::printClass(){
    //TODO
}

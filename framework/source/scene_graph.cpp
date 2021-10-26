#include "scene_graph.hpp"

std::string name = " ";
Node root;

std::string getName(){
    return name;
}
void setName(std::string n){
    name = n;
}

Node getRoot(){
    return root;
}

void setRoote(Node r){
    root = r;
}

std::string printGraph(){
    //TODO
}

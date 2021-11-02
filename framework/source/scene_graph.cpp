#include "scene_graph.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

Scene_graph::Scene_graph():
    name{" "}{}

Scene_graph::Scene_graph(std::string n){
    setName(n);
} 

Scene_graph::Scene_graph(std::string n, std::shared_ptr<Node> r){
    setRoot(r);
    setName(n);
} 
    /* Node root = Node("root");
    setRoot(root); */


std::string Scene_graph::getNameScene() const{
    return name;
}

void Scene_graph::setName(std::string n){
    name = n;
}

std::shared_ptr<Node> Scene_graph::getRoot() const{
    return root;
}

void Scene_graph::setRoot(std::shared_ptr<Node> r){
    root = r;
}

std::string printChildren(std::vector<std::shared_ptr<Node>> children){
    for(auto const& x : children){
        return x->getName();
        /* std::cout<< "hi" <<std::endl;
        std::cout<< x->getName() <<std::endl;
        std::cout<<x->getParent()->getName()<<" -> "<<x->getName()<<"->";
        printChildren(x->getChildrenList());
        std::cout<<std::endl;  */
    
        
    }  
    
}

std::string Scene_graph::printClass() const{
    //std::cout<<root->getName()<<"->";
    //std::cout<<root->getName()<<std::endl;
    std::cout<<printChildren(root->getChildrenList());
    
}

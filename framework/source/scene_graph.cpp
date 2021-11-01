#include <scene_graph.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

Scene_graph::Scene_graph():
    name{" "},
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
    std::cout<<root.getName()<<"->";
    for(std::vector<Node>::iterator it = root.getChildrenList().begin();it != root.getChildrenList().end(); ++it){
		std::cout<<it->getName()<<" -> ";
		//itorating the neighbour
		for(std::vector<Node>::iterator i = it->getChildrenList().begin(); i != it->getChildrenList().end(); ++i){
			std::cout<<i->getName();
			std::cout<<std::endl;
		}
	}
}

int main(int argc, char* argv[]){
    Node root(NULL,"root","???");
    Scene_graph scene_graph("test sceneGraph", root);
    Node node();
    scene_graph.printClass();
    return 0;
}
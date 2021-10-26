#include "node.hpp"

#include <vector>
#include <algorithm>

Node::Node():
    parent{},
    children{},
    name{"boring_default_name"},
    path{},
    depth{},
    localTransform{},
    worldTransform{}
    {}


Node Node::getParent(){
    return *parent;
}

void Node::setParent(Node *node){
    *parent = *node;
    return;
}

Node Node::getChildren(std::string name){
    std::vector<Node>::iterator it = std::find(children.begin(), children.end(), name);
    if(it == children.end()){
        throw;
    }
    return;
    //Todo need a way to iterate over the list to find the element (now vector)
}

std::vector<Node> Node::getChildrenList(){

}

std::string Node::getName(){

}

std::string Node::getPath(){

}

int Node::getDepth(){

}

glm::mat4 Node::getLocalTransform(){

}

void Node::setLocalTransform(glm::mat4){

}

glm::mat4 Node::getWorldTransform(){

}

void Node::setWorldTransform(glm::mat4){

}

void Node::addChildren(Node){

}

Node Node::removeChildren(std::string){

}
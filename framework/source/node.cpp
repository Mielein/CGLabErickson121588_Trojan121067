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

Node::Node(Node *_parent, std::string _name, std::string _path):
    parent{_parent},
    children{},
    name{_name},
    path{_path},
    depth{},
    localTransform{},
    worldTransform{}
    {}

Node Node::getParent(){
    return *parent;
}

void Node::setParent(Node *node){
    *parent = *node;
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
    return children;
}

std::string Node::getName(){
    return name;
}

std::string Node::getPath(){
    return path;
}

int Node::getDepth(){
    return depth;
}

glm::mat4 Node::getLocalTransform(){
    return localTransform;
}

void Node::setLocalTransform(glm::mat4 newLocal){
    localTransform = newLocal;
}

glm::mat4 Node::getWorldTransform(){
    return worldTransform;
}

void Node::setWorldTransform(glm::mat4 newWorld){
    worldTransform = newWorld;
}

void Node::addChildren(Node newNode){
    children.push_back(newNode);
}

Node Node::removeChildren(std::string){
    return;
}
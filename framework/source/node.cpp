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

Node Node::getParent() const{
    return *parent;
}

void Node::setParent(Node *node){
    *parent = *node;
}

Node Node::getChild(std::string name) const{
    for(Node const& x : children){
        if(x.getName() == name){
            return x;
        }
        getChild(name);
    }
        
    throw;
    //should be recursive
}

std::vector<Node> Node::getChildrenList() const{
    return children;
}

std::string Node::getName() const{
    return name;
}

std::string Node::getPath() const{
    return path;
}

int Node::getDepth() const{
    return depth;
}

glm::mat4 Node::getLocalTransform() const{
    return localTransform;
}

void Node::setLocalTransform(glm::mat4 newLocal){
    localTransform = newLocal;
}

glm::mat4 Node::getWorldTransform() const{
    return worldTransform;
}

void Node::setWorldTransform(glm::mat4 newWorld){
    worldTransform = newWorld;
}

void Node::addChildren(Node newNode){
    children.push_back(newNode);
}

Node Node::removeChildren(std::string name){
    Node tmp_Nope = getChild(name);
    Node tmp_Node; //ToDo
    
    return tmp_Node;
}
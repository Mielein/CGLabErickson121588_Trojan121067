#include "node.hpp"

#include <vector>
#include <algorithm>
#include <iostream>



Node::Node():
    name{"boring_default_name"}{}


Node::Node(std::string _name):
    name{_name}{}


std::shared_ptr<Node> Node::getParent() const{
    return parent;
}

void Node::setParent(std::shared_ptr<Node> node){
    parent = node;
    //std::cout << node->getName() << std::endl; 
}

Node Node::getChild(std::string name) const{
    for(auto const& x : children){
        auto tmp_parent = x->getParent();
        std::cout << tmp_parent->getName()<<" -> "<<x->getName() << std::endl; // red flags (maybe the corrupt output)
        if(x->getName() == name){
            return *x;
        }
        x->getChild(name);
    }
    throw;
    //should be recursive
}

std::vector<std::shared_ptr<Node>> Node::getChildrenList() const{
    //std::cout << "getChildrenList Callback" << std::endl;
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

void Node::addChild(std::shared_ptr<Node> newNode){
    Node tmp_node = *this;
    newNode->setParent(std::make_shared<Node>(tmp_node));
    children.insert(children.end(), newNode);
}

Node Node::removeChild(std::string name){
    Node tmp_Nope;
    Node tmp_Node; //ToDo
    for(auto const& x : children){
        if(x->getName() == name){
            tmp_Nope = *x;
        }
        getChild(name);
        tmp_Node = tmp_Nope;
        //children.erase(std::remove(children.begin(),children.end(),tmp_Nope));
    }
      
    return tmp_Node;
}
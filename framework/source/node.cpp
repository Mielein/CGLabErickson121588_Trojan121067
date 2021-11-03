#include "node.hpp"

#include <vector>
#include <algorithm>
#include <iostream>



Node::Node():
    name{"boring_default_name"}{}


Node::Node(std::string _name):
    name{_name}{}

Node::Node(std::string _name, glm::fmat4 _localTransform):
    name{_name},
    localTransform{_localTransform}{}

std::shared_ptr<Node> Node::getParent() const{
    //std::cout << "get Parent Call" << std::endl;
    //std::cout << parent->getName() << std::endl;
    Node tmp_Node("Error Node");
    if(parent != nullptr){
        return parent;
    }
    else
        return std::make_shared<Node>(tmp_Node);
}

void Node::setParent(std::shared_ptr<Node> node){
    parent = node;
    //std::cout << node->getName() << std::endl; 
}

std::shared_ptr<Node> Node::getChild(std::string name) const{
    if(children.size() == 0){
        return nullptr;
        //std::cout<<"sad"<<std::endl;
    }
    for(auto const& x : children){
        //std::cout<<"for loop"<<std::endl;
        //std::cout << x->getParent()->getName()<<" -> "<<x->getName() << std::endl; // red flags (maybe the corrupt output)
        if(x->getName() == name){
            //std::cout<<name<<std::endl;
            return x;

        }
        else if(x->getChild(name) == nullptr) continue;
        else return x->getChild(name);
    }
    return nullptr;
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
    std::string path;
    path = this->getName();
    for(auto const& x : this->getChildrenList()){
        x->getPath();
        path += " -> " + x->getName();;
    }
    return path;
}
/*     std::string path = this->getName();
    std::cout << " -> "; 
    for(auto const& x : children){
        if(children.size() != 0){
            return x->getName() + " ->" ;
        }
    } */



int Node::getDepth() const{
    return depth;
}

glm::fmat4 Node::getLocalTransform() const{
    return localTransform;
}

void Node::setLocalTransform(glm::fmat4 newLocal){
    localTransform = newLocal;
}

glm::fmat4 Node::getWorldTransform() const{
    return worldTransform;
}

void Node::setWorldTransform(glm::fmat4 newWorld){
    worldTransform = newWorld;
}

void Node::addChild(std::shared_ptr<Node> newNode){
    Node tmp_node = *this;
    newNode->setParent(std::make_shared<Node>(tmp_node));
    //std::cout << newNode->getParent()->getName() << std::endl;
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
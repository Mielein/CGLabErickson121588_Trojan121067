#include "node.hpp"

#include <vector>
#include <algorithm>
#include <iostream>


//defaultconstructor
Node::Node():
    name{"boring_default_name"}{}

Node::Node(std::string _name):
    name{_name}{}

Node::Node(std::string _name, std::shared_ptr<Node> _parent):
    name{_name},
    parent{_parent}{}

Node::Node(std::string _name, glm::fmat4 _localTransform):
    name{_name},
    localTransform{_localTransform}{}
    
Node::Node(std::string _name, std::shared_ptr<Node> _parent, glm::fmat4 _localTransform):
    name{_name},
    parent{_parent},
    localTransform{_localTransform}{}

//constructor for the root node 
Node::Node(std::string _name, glm::fmat4 _localTransform, glm::fmat4 _worldTransform):
    name{_name},
    localTransform{_localTransform},
    worldTransform{_worldTransform}{}

//checks if nullptr else return shared pointer to parent element
std::shared_ptr<Node> Node::getParent() const{ 
    //return parent;
    //std::cout << "get Parent Call" << std::endl;
    //std::cout << parent->getName() << std::endl;
    Node tmp_Node("Error Node");
    if(parent != nullptr){
        return parent;
    } 
    else 
        return std::make_shared<Node>(tmp_Node);
    
}

//gives new parent
void Node::setParent(std::shared_ptr<Node> node){
    parent = node;
    //std::cout << node->getName() << std::endl; 
}


std::shared_ptr<Node> Node::getChild(std::string name) const{
    if(children.size() == 0){
        //std::cout<< "this should not happen" << std::endl;
        return nullptr;
    }
    for(auto const& x : children){
        if(x->getName() == name){
            return x; //if no x is found it returns a nullptr

        }
        else if(x->getChild(name) == nullptr) continue;
        else return x->getChild(name);
    }
    return nullptr;
    //calls recursive
}

//no explanation nassessary
std::vector<std::shared_ptr<Node>> Node::getChildrenList() const{
    //std::cout << "getChildrenList Callback" << std::endl;
    return children;
}

//no explanation nassessary
std::string Node::getName() const{
    return name;
}

//calls recursive to check where a path is leading thoug
std::string Node::getPath() const{
    std::string path;
    path = this->getName();
    for(auto const& x : this->getChildrenList()){
        x->getPath();
        path += " -> " + x->getName();;
    }
    return path;
}

//returns size of path
int Node::getDepth() const{
    int depth = 1;
    for(auto const& x : this->getChildrenList()){
        if( x->getChildrenList().size() != 0){
            depth +=1;
        }
        for(auto const& y : x->getChildrenList()){
            if(x->getChildrenList().size() != 0){
                depth += 1;
            }
        }
    }
    return depth;
}

glm::fmat4 Node::getLocalTransform() const{
    return localTransform;
}

void Node::setLocalTransform(glm::fmat4 newLocal){
    localTransform = newLocal;
}

//worldtransform equals worldtransform of parent * localtransform
glm::fmat4 Node::getWorldTransform() const{
    if(parent == nullptr){
        return localTransform;
    }
    return parent->getWorldTransform() * localTransform;
}

void Node::setWorldTransform(glm::fmat4 newWorld){
    worldTransform = newWorld;
}

//set new parent for child and insert Node at end of node .child
void Node::addChild(std::shared_ptr<Node> newNode){
    //Node tmp_node = *this;
    //newNode->setParent(std::make_shared<Node>(*this/* tmp_node */));
    //std::cout<<"added "+ newNode->getName()<<std::endl;
    //std::cout << newNode->getParent()->getName() << std::endl;
    //std::cout<<tmp_node.getName() + " parent of " + newNode->getName()<<std::endl;
    //std::cout<<"not pushed"<<std::endl;
    children.push_back(newNode);
    //std::cout<<newNode->getParent()->getName()<<std::endl;
    //std::cout<<"pushed"<<std::endl;

}

//...//
Node Node::removeChild(std::string name){
    Node tmp_Nope;
    Node tmp_Node; 
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
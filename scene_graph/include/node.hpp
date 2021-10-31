#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "node.hpp"

#include <glm/gtc/type_precision.hpp>
#include <string>
#include <list>

class Node{
    private:
        Node *parent;
        std::vector<Node> children;               //List doesent work well, we use vector
        std::string name;
        std::string path;
        int depth;
        glm::mat4 localTransform;
        glm::mat4 worldTransform;
    public:
        Node();
        Node(Node *_parent, std::string _name, std::string _path);
        Node getParent();
        void setParent(Node *node);
        Node getChild(std::string name);
        std::vector<Node> getChildrenList();
        std::string getName();
        std::string getPath();
        int getDepth();
        glm::mat4 getLocalTransform();
        void setLocalTransform(glm::mat4 newLocal);
        glm::mat4 getWorldTransform();
        void setWorldTransform(glm::mat4 newWorld);
        void addChildren(Node newChild);
        Node removeChildren(std::string);
};

#endif
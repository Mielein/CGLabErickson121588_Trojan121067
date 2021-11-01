#ifndef NODE_HPP
#define NODE_HPP

#include <glm/gtc/type_precision.hpp>
#include <string>
#include <list>
#include <vector>

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
        Node getParent() const;
        void setParent(Node *node);
        Node getChild(std::string name) const;
        std::vector<Node> getChildrenList() const;
        std::string getName() const;
        std::string getPath() const;
        int getDepth() const;
        glm::mat4 getLocalTransform() const;
        void setLocalTransform(glm::mat4 newLocal);
        glm::mat4 getWorldTransform() const;
        void setWorldTransform(glm::mat4 newWorld);
        void addChildren(Node newChild);
        Node removeChildren(std::string name);
};

#endif
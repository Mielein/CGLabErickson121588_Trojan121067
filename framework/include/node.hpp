#ifndef NODE_HPP
#define NODE_HPP

#include <glm/gtc/type_precision.hpp>
#include <string>
#include <list>
#include <vector>
#include <memory>

class Node{
    private:
        std::shared_ptr<Node> parent;
        std::vector<std::shared_ptr<Node>> children;               //List doesent work well, we use vector
        std::string name;
        std::string path;
        int depth;
        glm::mat4 localTransform;
        glm::mat4 worldTransform;
    public:
        Node();
        Node(std::string _name);
        std::shared_ptr<Node> getParent() const;
        void setParent(std::shared_ptr<Node> node);
        Node getChild(std::string name) const;
        std::vector<std::shared_ptr<Node>> getChildrenList() const;
        std::string getName() const;
        std::string getPath() const;
        int getDepth() const;
        glm::mat4 getLocalTransform() const;
        void setLocalTransform(glm::mat4 newLocal);
        glm::mat4 getWorldTransform() const;
        void setWorldTransform(glm::mat4 newWorld);
        void addChild(std::shared_ptr<Node> newNode);
        Node removeChild(std::string name);
};

#endif
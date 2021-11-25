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
        glm::fmat4 localTransform;
        glm::fmat4 worldTransform;
        glm::vec3 colour;
    public:
        Node();
        Node(std::string _name);
        Node(std::string _name, glm::mat4 _localtransform);
        Node(std::string _name, std::shared_ptr<Node> _parent, glm::fmat4 _localTransform);
        Node(std::string _name, glm::fmat4 _localTransform, glm::fmat4 _worldTransform);
        Node(std::string _name, glm::fmat4 _localTransform, glm::fmat4 _worldTransform, glm::vec3 _colour);
        std::shared_ptr<Node> getParent() const;
        void setParent(std::shared_ptr<Node> node);
        std::shared_ptr<Node> getChild(std::string name) const;
        std::vector<std::shared_ptr<Node>> getChildrenList() const;
        std::string getName() const;
        std::string getPath() const;
        int getDepth() const;
        glm::fmat4 getLocalTransform() const;
        void setLocalTransform(glm::fmat4 newLocal);
        glm::fmat4 getWorldTransform() const;
        void setWorldTransform(glm::fmat4 newWorld);
        void addChild(std::shared_ptr<Node> newNode);
        Node removeChild(std::string name);
};

#endif
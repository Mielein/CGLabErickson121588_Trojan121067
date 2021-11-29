#ifndef CAMERA_NODE_HPP;
#define CAMERA_NODE_HPP;

#include "node.hpp"
#include <glm/glm.hpp>

class Camera_node : public Node{
    private:
        bool isPerspective;
        bool isEnabled;
        glm::mat4 projectionMatrix;
    public:
        Camera_node();
        Camera_node(std::string name);
        Camera_node(std::string name, glm::fmat4 _localTransform, std::shared_ptr<Node> _parent);
        bool getPerspective();
        bool getEnabled();
        void setEnable(bool enable);
        glm::mat4 getProjectionMatrix();
        void setProjectionMatrix(glm::mat4 matrix);
};

#endif
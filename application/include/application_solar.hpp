#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include <scene_graph.hpp>

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();
  //star initialisation
  void initializeStars();
  //orbit initialisation
  void initializeOrbits();
  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  void scrollCallback(double pos_x, double pos_y);
  void tmpfunk();

  // draw all objects
  void render() const;

  Scene_graph scene_graph_;

 protected:
  // initialize all objects in the graph to create the sun system
  void initializeSceneGraph();
  void initializeShaderPrograms();
  void initializeGeometry();
  
  // custom render methode for the planets out of the scene graph
  void planetrenderer() const;
  void starRenderer() const;
  void orbitRenderer() const;
  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();

  // cpu representation of model
  model_object planet_object;
  model_object star_object;
  model_object orbit_object;
  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;
  // initialized sceneGraph with information about objects and there rotation
};

#endif
#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"
#include <iostream>
#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,star_object{}
 ,orbit_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
 ,scene_graph_{}
{
  initializeSceneGraph();
  initializeGeometry();
  initializeStars();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteBuffers(1, &star_object.vertex_BO);
  glDeleteBuffers(1, &star_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
  glDeleteVertexArrays(1, &star_object.vertex_AO);
}

/////////////////////////////////////////////////////////////////////////////////////

void ApplicationSolar::initializeSceneGraph() {
  std::cout << "initialize Solar System" << std::endl;
  Node root_node("root element", glm::translate({}, glm::fvec3{1.0f, 0.0f, 0.0f }), glm::translate({}, glm::fvec3{1.0f, 0.0f, 0.0f }));
  
  Node stars_node("stars");

  Node mercury_node("Mercury", glm::translate({}, glm::fvec3{3.0f, 0.0f, 0.0f }));
  Node venus_node("Venus", glm::translate({}, glm::fvec3{14.0f, 0.0f, 0.0f }));
  Node earth_node("Earth", glm::translate({}, glm::fvec3{25.0f, 0.0f, 0.0f }));
  Node mars_node("Mars", glm::translate({}, glm::fvec3{36.0f, 0.0f, 0.0f }));
  Node jupiter_node("Jupiter", glm::translate({}, glm::fvec3{47.0f, 0.0f, 0.0f }));
  Node saturn_node("Saturn", glm::translate({}, glm::fvec3{58.0f, 0.0f, 0.0f }));
  Node urnaus_node("Uranus", glm::translate({}, glm::fvec3{69.0f, 0.0f, 0.0f }));
  Node neptune_node("Neptune", glm::translate({}, glm::fvec3{80.0f, 0.0f, 0.0f }));
  Node moon_node("Moon", glm::translate({}, glm::fvec3{5.3f, 0.0f, 0.0f }));

  Geometry_node mercury_geo("geo_Mercury", glm::translate({}, glm::fvec3{3.0f, 0.0f, 0.0f }));
  Geometry_node venus_geo("geo_Venus",glm::translate({}, glm::fvec3{14.0f, 0.0f, 0.0f }));
  Geometry_node earth_geo("geo_Earth", glm::translate({}, glm::fvec3{25.0f, 0.0f, 0.0f }));
  Geometry_node mars_geo("geo_Mars", glm::translate({}, glm::fvec3{36.0f, 0.0f, 0.0f }));
  Geometry_node jupiter_geo("geo_Jupiter", glm::translate({}, glm::fvec3{47.0f, 0.0f, 0.0f }));
  Geometry_node saturn_geo("geo_Saturn", glm::translate({}, glm::fvec3{58.0f, 0.0f, 0.0f }));
  Geometry_node urnaus_geo("geo_Uranus", glm::translate({}, glm::fvec3{69.0f, 0.0f, 0.0f }));
  Geometry_node neptune_geo("geo_Neptune", glm::translate({}, glm::fvec3{80.0f, 0.0f, 0.0f }));
  Geometry_node moon_geo("geo_Moon", glm::translate({}, glm::fvec3{5.3f, 0.0f, 0.0f }));

  Camera_node camera("Camera");
  
  earth_node.addChild(std::make_shared<Geometry_node>(earth_geo));
  mercury_node.addChild(std::make_shared<Geometry_node>(mercury_geo));
  venus_node.addChild(std::make_shared<Geometry_node>(venus_geo));
  mars_node.addChild(std::make_shared<Geometry_node>(mars_geo));
  jupiter_node.addChild(std::make_shared<Geometry_node>(jupiter_geo));
  saturn_node.addChild(std::make_shared<Geometry_node>(saturn_geo));
  urnaus_node.addChild(std::make_shared<Geometry_node>(urnaus_geo));
  neptune_node.addChild(std::make_shared<Geometry_node>(neptune_geo));
  moon_node.addChild(std::make_shared<Geometry_node>(moon_geo));
   
  earth_node.addChild(std::make_shared<Node>(moon_node));
  root_node.addChild(std::make_shared<Node>(stars_node));
  root_node.addChild(std::make_shared<Node>(mercury_node));
  root_node.addChild(std::make_shared<Node>(venus_node));
  root_node.addChild(std::make_shared<Node>(earth_node));
  root_node.addChild(std::make_shared<Node>(mars_node));
  root_node.addChild(std::make_shared<Node>(jupiter_node));
  root_node.addChild(std::make_shared<Node>(saturn_node));
  root_node.addChild(std::make_shared<Node>(urnaus_node));
  root_node.addChild(std::make_shared<Node>(neptune_node));
  root_node.addChild(std::make_shared<Node>(camera));

  scene_graph_= Scene_graph("Solar Scene Graph", root_node);
  scene_graph_.printClass();
}

void ApplicationSolar::initializeStars(){
  std::vector<GLfloat> stars;
  unsigned int star_count = 1000000;
  unsigned int max_distance = 1000;

  for(int i = 0; i < star_count; i++){

    GLfloat rand_x_pos = (std::rand() % max_distance)-500.0f;
    stars.push_back(rand_x_pos);
    GLfloat rand_y_pos = (std::rand() % max_distance)-500.0f;
    stars.push_back(rand_y_pos);
    GLfloat rand_z_pos = (std::rand() % max_distance)-500.0f;
    stars.push_back(rand_z_pos);
    //random colours
    // static_cast Returns a value of type new_type. 
    GLfloat rand_r_colour = static_cast<float>(std::rand() % 255)/255.0f;
    stars.push_back(rand_r_colour);
    GLfloat rand_g_colour = static_cast<float>(std::rand() % 255)/255.0f;
    stars.push_back(rand_g_colour);
    GLfloat rand_b_colour = static_cast<float>(std::rand() % 255)/255.0f;
    stars.push_back(rand_b_colour);

  }
  for(int i = 0; i < stars.size(); i++){
    //std::cout<<stars[i]<<std::endl;
  }
  
  //initialising Vertex array
  glGenVertexArrays(GLint(1), &star_object.vertex_AO);
  glBindVertexArray(star_object.vertex_AO);
  //initialising Vertex Buffer Object and load data
  glGenBuffers(GLuint(1), &star_object.vertex_BO);
  glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*stars.size(), stars.data(), GL_STATIC_DRAW);
  // first array attribut for positions
  glEnableVertexArrayAttrib(star_object.vertex_AO, 0);
  glVertexAttribPointer(GLuint(0), GLuint(3), GL_FLOAT, GL_FALSE, GLsizei(sizeof(float)*6), NULL);
  //second array for colours
  glEnableVertexArrayAttrib(star_object.vertex_AO, 1);
  glVertexAttribPointer(GLuint(1),GLuint(3),GL_FLOAT, GL_FALSE, GLsizei(sizeof(float)*6) , (void*)(sizeof(float)*3));

  star_object.draw_mode = GL_POINTS;
  star_object.num_elements = GLsizei(star_count);

}
////////////////////////////////////rendering/////////////////////////////////////////////////

void ApplicationSolar::render() const {
  
  // bind shader to upload uniforms
  starRenderer();
  planetrenderer();


}

////////////////////////////////////////////////////////////////////////////////////////////
void ApplicationSolar::starRenderer() const{
    glUseProgram(m_shaders.at("star").handle);
    glBindVertexArray(star_object.vertex_AO);
    glDrawArrays(star_object.draw_mode,GLint(0), star_object.num_elements);
    //glDrawElements(star_object.draw_mode, star_object.num_elements, model::INDEX.type, &star_object);
 }

void ApplicationSolar::planetrenderer() const{
  //render sun

  glUseProgram(m_shaders.at("planet").handle);
  glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 0.0f, 1.0f});
  model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -1.0f});
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));

  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);


  std::vector<std::shared_ptr<Node>> List_of_Planets;
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Mercury"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Venus"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Earth"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Mars"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Jupiter"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Saturn"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Uranus"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Neptune"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Moon"));
  int tmp = 1;
  for(std::shared_ptr<Node> x : List_of_Planets){
    //std::cout << x->getName();
    glUseProgram(m_shaders.at("planet").handle);
   
    glm::fmat4 final_matrix;

    //getting the Geometry-node equivalent to Node x
    std::shared_ptr<Node> planet_geo = x->getChild("geo_" + x->getName());
    //initializes Matrix with localTransform of Parent of x
    //glm::fmat4 parent_matrix = x->getParent()->getLocalTransform();
    //We set our orientation source to the local transform of the parent because we want our planets to rotate around their parent
    glm::fmat4 rotation_matrix = glm::rotate(glm::fmat4{}/* planet_geo->getParent()->getLocalTransform() */, 0.0001f*tmp ,glm::fvec3{0.0f, 1.0f, 0.0f});
    //we multiply LocalTransform of the Geometry Node and the rotation Matrix and set it as their parents localTransform,
    //this way x sees the parent as the center of the orbit
    glm::fmat4 newTransform = rotation_matrix * planet_geo->getParent()->getLocalTransform(); 
    
    planet_geo->getParent()->setLocalTransform(newTransform);
    
    //final_matrix = glm::rotate(planet_geo->getParent()->getWorldTransform(), float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});
    final_matrix = planet_geo->getWorldTransform();
    tmp++;

    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(final_matrix));
    
    final_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * final_matrix);

    glBindVertexArray(planet_object.vertex_AO);

    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
    
  }
}



////////////////////////////////////////////////////////////////////////////////////////////

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  glUseProgram(m_shaders.at("star").handle);
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ModelViewMatrix"),
                      1, GL_FALSE, glm::value_ptr(view_matrix)); 


  glUseProgram(m_shaders.at("planet").handle);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix)); 
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
  glUseProgram(m_shaders.at("star").handle);
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ProjectionMatrix"),
                      1, GL_FALSE, glm::value_ptr(m_view_projection));

 
} 

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // upload uniform values to new locations

  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;

  m_shaders.emplace("star", shader_program{{{GL_VERTEX_SHADER, m_resource_path + "shaders/vao.vert"},
                                            {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}}});
  m_shaders.at("star").u_locs["ModelViewMatrix"] = -1;
  m_shaders.at("star").u_locs["ProjectionMatrix"] = -1;

}

// load models
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/fred_the_frog.obj", model::NORMAL);

  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(planet_model.indices.size());
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  }
  else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.1f, 0.0f, 0.0f});
    uploadView();
  }
    else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.1f, 0.0f, 0.0f});
    uploadView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  float currentFrame = glfwGetTime();
  float lastFrame = 0;
  float deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  //y-axis rotation
  m_view_transform = glm::rotate(m_view_transform, float(((pos_x/-100.0f)*glm::radians(deltaTime))),glm::fvec3{0.0f, 1.0f, 0.0f});
  //x-axix rotation
  m_view_transform = glm::rotate(m_view_transform, float(((pos_y/-100.0f)*glm::radians(deltaTime))),glm::fvec3{1.0f, 0.0f, 0.0f});

  uploadView();
/*     if(pos_x < 0){
      m_view_transform = glm::rotate(m_view_transform, glm::radians(deltaTime)/100 ,glm::fvec3{-0.1f, 0.0f, 0.0f});
      uploadView();
    } 
    if(pos_x > 0){
      m_view_transform = glm::rotate(m_view_transform, glm::radians(deltaTime)/100 ,glm::fvec3{0.1f, -0.1f, 0.1f});
      uploadView();
    }
    if(pos_y < 0){
      m_view_transform = glm::rotate(m_view_transform, glm::radians(deltaTime)/100,glm::fvec3{0.0f, -0.1f, 0.0f});
      uploadView();
    } 
    if(pos_y > 0){
      m_view_transform = glm::rotate(m_view_transform, glm::radians(deltaTime)/100,glm::fvec3{0.0f, 0.1f,0.0f});
      uploadView();
    } */


}

void ApplicationSolar::scrollCallback(double pos_x, double pos_y){

}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}


// exe entry point
int main(int argc, char* argv[]) {

  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}
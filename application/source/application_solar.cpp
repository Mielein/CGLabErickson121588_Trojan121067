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
#include <glm/gtx/string_cast.hpp>

#include <iostream>

void debugPrint(std::string string){
  std::cout<<string<<std::endl;
}
ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,star_object{}
 ,orbit_object{}
 ,framebuffer_obj{}
 ,texture_object{}
 ,skybox_object{}
 ,quad_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
 ,m_width{initial_resolution.x}
 ,m_height{initial_resolution.y}
 ,scene_graph_{}
{
  initializeSceneGraph();
  initializeGeometry();
  initializeStars();
  initializeOrbits();
  initializeTextures();
  initializeSkybox();
  initializeSun();
  initializeShaderPrograms();

  initializeFramebuffer(m_width, m_height);
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteBuffers(1, &star_object.vertex_BO);
  glDeleteBuffers(1, &star_object.element_BO);
  glDeleteBuffers(1, &skybox_object.vertex_BO);
  glDeleteBuffers(1, &skybox_object.element_BO);
  glDeleteBuffers(1, &quad_object.vertex_BO);
  glDeleteBuffers(1, &quad_object.element_BO);
  glDeleteVertexArrays(1, &quad_object.vertex_AO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
  glDeleteVertexArrays(1, &star_object.vertex_AO);
  glDeleteVertexArrays(1, &skybox_object.vertex_AO);
}
unsigned int m_texture;
unsigned int m_sunTexture;
unsigned int m_skytextures;
float m_width;
float m_height;

/////////////////////////////////////////////////////////////////////////////////////
void ApplicationSolar::tmpfunk(){
  std::cout<< "this is tmpfunk" << std::endl;
}

void ApplicationSolar::initializeSceneGraph() {
  std::cout << "initialize Solar System" << std::endl;
  Node root_node("root element", glm::translate({}, glm::fvec3{0.0f, 0.0f, 0.0f }), glm::translate({}, glm::fvec3{1.0f, 0.0f, 0.0f }));
  //Node stars_node("stars");
  Node orbit_node("orbit");
  Camera_node camera("Camera",glm::transpose(glm::inverse(m_view_transform)) , std::make_shared<Node>(root_node));

  Node mercury_node("Mercury", std::make_shared<Node>(root_node), glm::translate({}, glm::fvec3{14.0f, 0.0f, 0.0f }),{0.8f,0.8f,0.8f}, true);
  Node venus_node("Venus", std::make_shared<Node>(root_node), glm::translate({}, glm::fvec3{18.0f, 0.0f, 0.0f }),{0.99f,0.8f,0.8f}, true);
  Node earth_node("Earth", std::make_shared<Node>(root_node), glm::translate({}, glm::fvec3{22.0f, 0.0f, 0.0f }),{0.0f,1.0f,1.0f}, true);
  Node mars_node("Mars", std::make_shared<Node>(root_node), glm::translate({}, glm::fvec3{26.0f, 0.0f, 0.0f }),{1.0f,0.8f,0.8f}, true);
  Node jupiter_node("Jupiter",std::make_shared<Node>(root_node), glm::translate({}, glm::fvec3{34.0f, 0.0f, 0.0f }),{0.8f,0.5f,0.5f}, true);
  Node saturn_node("Saturn",std::make_shared<Node>(root_node), glm::translate({}, glm::fvec3{42.0f, 0.0f, 0.0f }),{1.0f,1.0f,0.0f}, true);
  Node urnaus_node("Uranus",std::make_shared<Node>(root_node), glm::translate({}, glm::fvec3{50.0f, 0.0f, 0.0f }),{0.0f,0.0f,1.0f}, true);
  Node neptune_node("Neptune",std::make_shared<Node>(root_node), glm::translate({}, glm::fvec3{58.0f, 0.0f, 0.0f }),{0.2f,0.2f,1.0f}, true);

  root_node.setTexture(m_resource_path + "textures/sunmap.png");
  mercury_node.setTexture(m_resource_path + "textures/mercurymap.png");
  mercury_node.setMapping(m_resource_path + "textures/mercury_normal_map.png");
  venus_node.setTexture(m_resource_path + "textures/venusmap.png");
  venus_node.setMapping(m_resource_path + "textures/venus_normal_map.png");
  earth_node.setTexture(m_resource_path + "textures/earthmap1k.png");
  earth_node.setMapping(m_resource_path + "textures/earth_normal_map.png");
  mars_node.setTexture(m_resource_path + "textures/mars_1k_color.png");
  mars_node.setMapping(m_resource_path + "textures/mars_normal_map.png");
  jupiter_node.setTexture(m_resource_path + "textures/jupitermap.png");
  jupiter_node.setMapping(m_resource_path + "textures/jupiter_normal_map.png");
  saturn_node.setTexture(m_resource_path + "textures/saturnmap.png");
  saturn_node.setMapping(m_resource_path + "textures/saturn_normal_map.png");
  urnaus_node.setTexture(m_resource_path + "textures/uranusmap.png");
  urnaus_node.setMapping(m_resource_path + "textures/uranus_normal_map.png");
  neptune_node.setTexture(m_resource_path + "textures/neptunemap.png");
  neptune_node.setMapping(m_resource_path + "textures/neptune_normal_map.png");  

  float bigger = 3.3f;

  Geometry_node mercury_geo("geo_Mercury", std::make_shared<Node>(mercury_node), glm::scale({}, glm::fvec3{0.1f, 0.1f, 0.1f } * bigger));
  Geometry_node venus_geo("geo_Venus", std::make_shared<Node>(venus_node), glm::scale({}, glm::fvec3{0.14f, 0.14f, 0.14f }* bigger));
  Geometry_node earth_geo("geo_Earth", std::make_shared<Node>(earth_node), glm::scale({}, glm::fvec3{0.16f, 0.16f, 0.16f }* bigger));
  Geometry_node mars_geo("geo_Mars", std::make_shared<Node>(mars_node), glm::scale({}, glm::fvec3{0.13f, 0.13f, 0.13f }* bigger));
  Geometry_node jupiter_geo("geo_Jupiter", std::make_shared<Node>(jupiter_node), glm::scale({}, glm::fvec3{0.3f, 0.3f, 0.3f }* bigger));
  Geometry_node saturn_geo("geo_Saturn", std::make_shared<Node>(saturn_node), glm::scale({}, glm::fvec3{0.25f, 0.25f, 0.25f }* bigger));
  Geometry_node urnaus_geo("geo_Uranus", std::make_shared<Node>(urnaus_node), glm::scale({}, glm::fvec3{0.5f, 0.5f, 0.5f }* bigger));
  Geometry_node neptune_geo("geo_Neptune", std::make_shared<Node>(neptune_node), glm::scale({}, glm::fvec3{0.45f, 0.45f, 0.45f }* bigger));
  Node moon_node("Moon",std::make_shared<Node>(earth_node), glm::translate({}, glm::fvec3{1.0f, 0.0f, 0.0f }), {0.5f,0.5f,0.5f}, true);   // using boolean to know if using Mapping
  moon_node.setTexture(m_resource_path + "textures/moon.png");
  moon_node.setMapping(m_resource_path + "textures/moon_normal_map.png"); //setting mapping texture
  Geometry_node moon_geo("geo_Moon", std::make_shared<Node>(moon_node), glm::scale({}, glm::fvec3{0.08f, 0.08f, 0.08f }* bigger));

  Geometry_node moon_geo_orbit("geo_Moon_orbit", std::make_shared<Node>(moon_node), glm::translate({}, glm::fvec3{0.0f, 0.0f, 0.0f }));
  Geometry_node mercury_geo_orbit("geo_Mercury_orbit", std::make_shared<Node>(mercury_node), glm::translate({}, glm::fvec3{0.0f, 0.0f, 0.0f }));
  Geometry_node venus_geo_orbit("geo_Venus_orbit", std::make_shared<Node>(venus_node), glm::translate({}, glm::fvec3{0.0f, 0.0f, 0.0f }));
  Geometry_node earth_geo_orbit("geo_Earth_orbit", std::make_shared<Node>(earth_node), glm::translate({}, glm::fvec3{0.0f, 0.0f, 0.0f }));
  Geometry_node mars_geo_orbit("geo_Mars_orbit", std::make_shared<Node>(mars_node), glm::translate({}, glm::fvec3{0.0f, 0.0f, 0.0f }));
  Geometry_node jupiter_geo_orbit("geo_Jupiter_orbit", std::make_shared<Node>(jupiter_node), glm::translate({}, glm::fvec3{0.0f, 0.0f, 0.0f }));
  Geometry_node saturn_geo_orbit("geo_Saturn_orbit", std::make_shared<Node>(saturn_node), glm::translate({}, glm::fvec3{0.0f, 0.0f, 0.0f }));
  Geometry_node urnaus_geo_orbit("geo_Uranus_orbit", std::make_shared<Node>(urnaus_node), glm::translate({}, glm::fvec3{0.0f, 0.0f, 0.0f }));
  Geometry_node neptune_geo_orbit("geo_Neptune_orbit", std::make_shared<Node>(neptune_node), glm::translate({}, glm::fvec3{0.0f, 0.0f, 0.0f }));

  Point_light_node schimmer("Schimmer", glm::translate({}, glm::fvec3{0.0f, 0.0f, 0.0f }), std::make_shared<Node>(root_node), glm::vec3{1.0f, 1.0f, 1.0f}, 6500.0f);

  earth_node.addChild(std::make_shared<Geometry_node>(earth_geo));
  mercury_node.addChild(std::make_shared<Geometry_node>(mercury_geo));
  venus_node.addChild(std::make_shared<Geometry_node>(venus_geo));
  mars_node.addChild(std::make_shared<Geometry_node>(mars_geo));
  jupiter_node.addChild(std::make_shared<Geometry_node>(jupiter_geo));
  saturn_node.addChild(std::make_shared<Geometry_node>(saturn_geo));
  urnaus_node.addChild(std::make_shared<Geometry_node>(urnaus_geo));
  neptune_node.addChild(std::make_shared<Geometry_node>(neptune_geo));
  moon_node.addChild(std::make_shared<Geometry_node>(moon_geo));
  moon_node.addChild(std::make_shared<Geometry_node>(moon_geo_orbit));
  earth_node.addChild(std::make_shared<Node>(moon_node));

  earth_node.addChild(std::make_shared<Geometry_node>(earth_geo_orbit));
  mercury_node.addChild(std::make_shared<Geometry_node>(mercury_geo_orbit));
  venus_node.addChild(std::make_shared<Geometry_node>(venus_geo_orbit));
  mars_node.addChild(std::make_shared<Geometry_node>(mars_geo_orbit));
  jupiter_node.addChild(std::make_shared<Geometry_node>(jupiter_geo_orbit));
  saturn_node.addChild(std::make_shared<Geometry_node>(saturn_geo_orbit));
  urnaus_node.addChild(std::make_shared<Geometry_node>(urnaus_geo_orbit));
  neptune_node.addChild(std::make_shared<Geometry_node>(neptune_geo_orbit));
   
  //root_node.addChild(std::make_shared<Node>(stars_node));
  
  root_node.addChild(std::make_shared<Node>(mercury_node));
  root_node.addChild(std::make_shared<Node>(venus_node));
  root_node.addChild(std::make_shared<Node>(earth_node));
  root_node.addChild(std::make_shared<Node>(mars_node));
  root_node.addChild(std::make_shared<Node>(jupiter_node));
  root_node.addChild(std::make_shared<Node>(saturn_node));
  root_node.addChild(std::make_shared<Node>(urnaus_node));
  root_node.addChild(std::make_shared<Node>(neptune_node));
  root_node.addChild(std::make_shared<Point_light_node>(schimmer));
  root_node.addChild(std::make_shared<Node>(camera));
   

  scene_graph_= Scene_graph("Solar Scene Graph", root_node);
  //root_node.addChild(std::make_shared<Node>(orbit_node));

  std::cout<<scene_graph_.getRoot().getName()<<std::endl;
  std::cout<<scene_graph_.getRoot().getChild("Earth")->getChildrenList().size()<<std::endl;
  std::cout<<root_node.getChildrenList().size()<<std::endl;
  
  std::cout<<earth_geo.getParent()->getName()<<std::endl;
  std::cout<<earth_node.getParent()->getName()<<std::endl;
  std::cout<<earth_geo.getParent()->getParent()->getName()<<std::endl;

  scene_graph_.printClass();

}

//--------------------------------------------------------FRAMEBUFFER--------------------------------------------------------------------
void ApplicationSolar::initializeFramebuffer(unsigned width, unsigned height){
  debugPrint("Initialising Framebuffer");
  //Define Framebuffer
  glGenFramebuffers(1, &framebuffer_obj.handle);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_obj.handle);

  //create colour attachment
  //glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &framebuffer_obj.fbo_tex_handle);
  glBindTexture(GL_TEXTURE_2D, framebuffer_obj.fbo_tex_handle);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB , width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  glBindTexture(GL_TEXTURE_2D, 0);

  //Define Attachments (one call for each attachment to be defined)
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_obj.fbo_tex_handle,  0);

  //create renderbuffer attachment
  glGenRenderbuffers(1, &framebuffer_obj.rb_handle);
  glBindRenderbuffer(GL_RENDERBUFFER, framebuffer_obj.rb_handle);
  //GL_DEPTH_COMPONENT24 specifies the number of color components in the texture
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height); //not sure if initialres. is okay
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebuffer_obj.rb_handle);

  //Check that the Framebuffer can be written; hence, that...
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE ){
    debugPrint("ERROR: FRAMEBUFFER. not complete!");
  }
  
  debugPrint("Framebuffer Initialised");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//--------------------------------------------------------------------------------------------------------------------------------------

void ApplicationSolar::initializeStars(){

  std::vector<GLfloat> stars;
  //number stars
  unsigned int star_count = 10000;
  //range of spawn
  unsigned int max_distance = 1000;

  for(int i = 0; i < star_count; i++){

    GLfloat rand_x_pos = (std::rand() % max_distance)-500.0f;// -500 is middle value to spread stars evenly
    stars.push_back(rand_x_pos);
    GLfloat rand_y_pos = (std::rand() % max_distance)-500.0f;// -500 is middle value to spread stars evenly
    stars.push_back(rand_y_pos);
    GLfloat rand_z_pos = (std::rand() % max_distance)-500.0f;// -500 is middle value to spread stars evenly
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



  //setting the draw mode to GL_POINTS 
  star_object.draw_mode = GL_POINTS;
  star_object.num_elements = GLsizei(star_count);

}

//initialize Textures
//-----------------------------------------------------------------------------------------------------------------------------------------

void ApplicationSolar::initializeSun(){
  pixel_data sun_data;
  try{
      sun_data = texture_loader::file(m_resource_path + "textures/sunmap.png");
    }
    catch(std::exception e){
      std::cout<<"texture could not load for sun"<<std::endl;
    }
  //Initialise Texture
    glActiveTexture(GL_TEXTURE10);
    glGenTextures(1, &m_sunTexture);
    glBindTexture(GL_TEXTURE_2D, m_sunTexture);
    //Define Texture Sampling Parameters (mandatory)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //Define Texture Data and Format
/*     std::cout<<"texture: "<< m_sunTexture<<std::endl;
    std::cout<<"channel_type: "<< sun_data.channel_type<<std::endl;
    std::cout<<"width: "<< sun_data.width<<std::endl;
    std::cout<<"height: "<< sun_data.height<<std::endl;
    std::cout<<"channels: "<< sun_data.channels<<std::endl;  */

    glTexImage2D(GL_TEXTURE_2D, 0, sun_data.channels , sun_data.width, sun_data.height, 0,
    sun_data.channels, sun_data.channel_type, sun_data.ptr());
}

void ApplicationSolar::initializeTextures(){
  std::vector<std::shared_ptr<Node>> list_of_Planets;
  list_of_Planets.push_back(scene_graph_.getRoot().getChild("Mercury"));
  list_of_Planets.push_back(scene_graph_.getRoot().getChild("Venus"));
  list_of_Planets.push_back(scene_graph_.getRoot().getChild("Earth"));
  list_of_Planets.push_back(scene_graph_.getRoot().getChild("Mars"));
  list_of_Planets.push_back(scene_graph_.getRoot().getChild("Jupiter"));
  list_of_Planets.push_back(scene_graph_.getRoot().getChild("Saturn"));
  list_of_Planets.push_back(scene_graph_.getRoot().getChild("Uranus"));
  list_of_Planets.push_back(scene_graph_.getRoot().getChild("Neptune"));
  list_of_Planets.push_back(scene_graph_.getRoot().getChild("Moon")); 

  for(auto p : list_of_Planets){
    pixel_data planet_data;
    try{
      planet_data = texture_loader::file(p->getTexture());
    }
    catch(std::exception e){
      std::cout<<"texture could not load for " + p->getName()<<std::endl;
    }
    //debugPrint(p->getTexture());
    //Initialise Texture
    //glActiveTexture(GL_TEXTURE1+planet);
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    //Define Texture Sampling Parameters (mandatory)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //Define Texture Data and Format
/*     std::cout<<"texture: "<< m_texture<<std::endl;
    std::cout<<"channel_type: "<< planet_data.channel_type<<std::endl;
    std::cout<<"width: "<< planet_data.width<<std::endl;
    std::cout<<"height: "<< planet_data.height<<std::endl;
    std::cout<<"channels: "<< planet_data.channels<<std::endl;  */

    glTexImage2D(GL_TEXTURE_2D, 0, planet_data.channels , (GLsizei)planet_data.width, (GLsizei)planet_data.height, 0,
    planet_data.channels, planet_data.channel_type, planet_data.ptr());

    p->setTexInt(m_texture);

    //setting mapping texture?
    if(p->is_using_mapping()){
        try{
        planet_data = texture_loader::file(p->getMapping());
      }
      catch(std::exception e){
        std::cout<<"texture could not load for " + p->getName()<<std::endl;
      }
      //debugPrint(p->getTexture());
      //Initialise Texture
        //glActiveTexture(GL_TEXTURE1+planet);
        glGenTextures(1, &m_mappingtexture);
        glBindTexture(GL_TEXTURE_2D, m_mappingtexture);
        //Define Texture Sampling Parameters (mandatory)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      
      //Define Texture Data and Format
  /*     std::cout<<"texture: "<< m_texture<<std::endl;
      std::cout<<"channel_type: "<< planet_data.channel_type<<std::endl;
      std::cout<<"width: "<< planet_data.width<<std::endl;
      std::cout<<"height: "<< planet_data.height<<std::endl;
      std::cout<<"channels: "<< planet_data.channels<<std::endl;  */

      glTexImage2D(GL_TEXTURE_2D, 0, planet_data.channels , (GLsizei)planet_data.width, (GLsizei)planet_data.height, 0,
      planet_data.channels, planet_data.channel_type, planet_data.ptr());

      p->setMappingInt(m_mappingtexture);
    }
  }
}

void ApplicationSolar::initializeSkybox(){

  float skybox_points[] = {
  -1000.0f, -1000.0f, -1000.0f,
  -1000.0f,  1000.0f, -1000.0f,
   1000.0f, -1000.0f, -1000.0f,
   1000.0f, -1000.0f, -1000.0f,
   1000.0f,  1000.0f, -1000.0f,
  -1000.0f,  1000.0f, -1000.0f,
  
  -1000.0f, -1000.0f,  1000.0f,
  -1000.0f, -1000.0f, -1000.0f,
  -1000.0f,  1000.0f, -1000.0f,
  -1000.0f,  1000.0f, -1000.0f,
  -1000.0f,  1000.0f,  1000.0f,
  -1000.0f, -1000.0f,  1000.0f,
  
   1000.0f, -1000.0f, -1000.0f,
   1000.0f,  -1000.0f,  1000.0f,
   1000.0f,  1000.0f,  1000.0f,
   1000.0f,  1000.0f,  1000.0f,
   1000.0f,  1000.0f, -1000.0f,
   1000.0f, -1000.0f, -1000.0f,
   
  -1000.0f, -1000.0f,  1000.0f,
  -1000.0f,  1000.0f,  1000.0f,
   1000.0f,  1000.0f,  1000.0f,
   1000.0f,  1000.0f,  1000.0f,
   1000.0f, -1000.0f,  1000.0f,
  -1000.0f, -1000.0f,  1000.0f,
  
  -1000.0f,  1000.0f, -1000.0f,
   1000.0f,  1000.0f, -1000.0f,
   1000.0f,  1000.0f,  1000.0f,
   1000.0f,  1000.0f,  1000.0f,
  -1000.0f,  1000.0f,  1000.0f,
  -1000.0f,  1000.0f, -1000.0f,
  
  -1000.0f, -1000.0f, -1000.0f,
  -1000.0f, -1000.0f,  1000.0f,
   1000.0f, -1000.0f, -1000.0f,
   1000.0f, -1000.0f, -1000.0f,
  -1000.0f, -1000.0f,  1000.0f,
   1000.0f, -1000.0f,  1000.0f
};
  glGenVertexArrays(GLint(1), &skybox_object.vertex_AO);
  glBindVertexArray(skybox_object.vertex_AO);

  glGenBuffers(GLuint(1), &skybox_object.vertex_BO);
  glBindBuffer(GL_ARRAY_BUFFER, skybox_object.vertex_BO);
  glBufferData(GL_ARRAY_BUFFER, 3*36 *sizeof(float), &skybox_points, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(GLuint(0), GLuint(3), GL_FLOAT, GL_FALSE, 0, NULL);


  glActiveTexture(GL_TEXTURE10);
  glGenTextures(1, &m_skytextures);
  debugPrint(std::to_string(m_skytextures));
  pixel_data sky_data_front = texture_loader::file(m_resource_path + "textures/right1.png");
  pixel_data sky_data_back = texture_loader::file(m_resource_path + "textures/left1.png");
  pixel_data sky_data_top = texture_loader::file(m_resource_path + "textures/bottom1.png");
  pixel_data sky_data_bottom = texture_loader::file(m_resource_path + "textures/top1.png");
  pixel_data sky_data_left = texture_loader::file(m_resource_path + "textures/back1.png");
  pixel_data sky_data_right = texture_loader::file(m_resource_path + "textures/front1.png");

      
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, sky_data_right.channels, sky_data_right.width, sky_data_right.height, 0, 
  sky_data_right.channels, sky_data_right.channel_type, sky_data_right.ptr());
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, sky_data_top.channels, sky_data_top.width, sky_data_top.height, 0, 
  sky_data_top.channels, sky_data_top.channel_type, sky_data_top.ptr());
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, sky_data_back.channels, sky_data_back.width, sky_data_back.height, 0, 
  sky_data_back.channels, sky_data_back.channel_type, sky_data_back.ptr());
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, sky_data_left.channels, sky_data_left.width, sky_data_left.height, 0, 
  sky_data_left.channels, sky_data_left.channel_type, sky_data_left.ptr());
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, sky_data_bottom.channels, sky_data_bottom.width, sky_data_bottom.height, 0, 
  sky_data_bottom.channels, sky_data_bottom.channel_type, sky_data_bottom.ptr());
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, sky_data_front.channels, sky_data_front.width, sky_data_front.height, 0, 
  sky_data_front.channels, sky_data_front.channel_type, sky_data_front.ptr());


  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_CUBE_MAP, m_skytextures);
 
  debugPrint(std::to_string(m_skytextures));
  skybox_object.num_elements = 36;
  skybox_object.draw_mode = GL_TRIANGLES;

  debugPrint(std::to_string(skybox_object.vertex_AO));
}

//create Orbits
//-----------------------------------------------------------------------------------------------------------------------------------------

void ApplicationSolar::initializeOrbits(){
  //points per orbit
  int numOrbitPoints = 100;
  std::vector<GLfloat> orbits;
  //verctor containing all the orbit nodes, that are previously been initialize in the scene graph
  std::vector<std::shared_ptr<Node>> list_of_geoPlanets;
  list_of_geoPlanets.push_back(scene_graph_.getRoot().getChild("geo_Mercury_orbit"));
  list_of_geoPlanets.push_back(scene_graph_.getRoot().getChild("geo_Venus_orbit"));
  list_of_geoPlanets.push_back(scene_graph_.getRoot().getChild("geo_Earth_orbit"));
  list_of_geoPlanets.push_back(scene_graph_.getRoot().getChild("geo_Mars_orbit"));
  list_of_geoPlanets.push_back(scene_graph_.getRoot().getChild("geo_Jupiter_orbit"));
  list_of_geoPlanets.push_back(scene_graph_.getRoot().getChild("geo_Saturn_orbit"));
  list_of_geoPlanets.push_back(scene_graph_.getRoot().getChild("geo_Uranus_orbit"));
  list_of_geoPlanets.push_back(scene_graph_.getRoot().getChild("geo_Neptune_orbit"));
  list_of_geoPlanets.push_back(scene_graph_.getRoot().getChild("geo_Moon_orbit"));

  std::vector<glm::vec3> scaling_values;

/*   for(auto x : list_of_geoPlanets){
    debugPrint(x->getParent()->getName() + ": " + glm::to_string(x->getParent()->getLocalTransform()));
  } */
  //some good ol' hardcoding
  float make_smol = 0.07152f;

  scaling_values.push_back(glm::vec3{14.0f, 14.0f, 14.0f} * make_smol);
  scaling_values.push_back(glm::fvec3{18.0f, 18.0f, 18.0f }* make_smol);
  scaling_values.push_back(glm::vec3{22.0f, 22.0f, 22.0f}* make_smol);
  scaling_values.push_back(glm::vec3{26.0f, 26.0f, 26.0f}* make_smol);
  scaling_values.push_back(glm::vec3{34.0f, 34.0f, 34.0f}* make_smol);
  scaling_values.push_back(glm::vec3{42.0f, 42.0f, 42.0f}* make_smol);
  scaling_values.push_back(glm::vec3{50.0f, 50.0f, 50.0f}* make_smol);
  scaling_values.push_back(glm::vec3{58.0f, 58.0f, 58.0f}* make_smol);
  scaling_values.push_back(glm::vec3{0.08f, 0.08f, 0.08f}* make_smol);

  unsigned int counter = 0; 

  //this loop pushes the elements for each orbit to its geometry
  for(std::shared_ptr<Node> x : list_of_geoPlanets){
    auto planet = x->getParent();
    glm::fvec4 point = planet->getLocalTransform()* glm::fvec4{0.0f,0.0f,0.0f,1.0f};
    glm::fmat4 rotation_matrix = glm::rotate(glm::fmat4{}, 0.1f,glm::fvec3{0.0f, 1.0f, 0.0f});
    for(int i = 0; i< numOrbitPoints; i++){
      orbits.push_back(point.x);
      orbits.push_back(point.y);
      orbits.push_back(point.z);
      point = rotation_matrix * point ;
    }

    x->setLocalTransform(x->getLocalTransform() * glm::scale({}, scaling_values[counter]));
    model orbit_model; 
    orbit_model.data = orbits; 
    orbit_model.vertex_num = numOrbitPoints;
    std::static_pointer_cast<Geometry_node>(x)->setGeometry(orbit_model);
    counter++;

  }

  glGenVertexArrays(GLint(1), &orbit_object.vertex_AO);
  glBindVertexArray(orbit_object.vertex_AO);
  //initialising Vertex Buffer Object and load data
  glGenBuffers(GLuint(1), &orbit_object.vertex_BO);
  glBindBuffer(GL_ARRAY_BUFFER, orbit_object.vertex_BO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*orbits.size(), orbits.data(), GL_STATIC_DRAW);
  // first array attribut for positions
  glEnableVertexArrayAttrib(orbit_object.vertex_AO, 0);
  glVertexAttribPointer(GLuint(0), GLuint(3), GL_FLOAT, GL_FALSE, GLsizei(sizeof(float)*3), 0);

  // we use the GL_LINE_STRIP to draw a free line
  orbit_object.draw_mode = GL_LINE_STRIP;
  orbit_object.num_elements = GLsizei(numOrbitPoints);
}
////////////////////////////////////rendering/////////////////////////////////////////////////

void ApplicationSolar::render() {
  //bind offscreen framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_obj.handle);
  //clear framebuffer
  glClearColor(0.1f,0.1f,0.1f,0.1f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  // bind shader to upload uniforms
  skyboxrenderer();
  starRenderer();
  planetrenderer();
  orbitRenderer();

  //render default framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //clear default framebuffer
  glClearColor(1.0f,1.0f,1.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT); //not using depthbuffer, so no need to clear that;
  glDisable(GL_DEPTH_TEST);
  
  //render quad
  glUseProgram(m_shaders.at("quad").handle);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, framebuffer_obj.fbo_tex_handle);
  
  int screentex_location = glGetUniformLocation(m_shaders.at("quad").handle, "screen_texture");
/*  int inverse_check = glGetUniformLocation(m_shaders.at("quad").handle, "inverse");
  int grayscale_check = glGetUniformLocation(m_shaders.at("quad").handle, "grayscale_on");
  int mirror_v_check = glGetUniformLocation(m_shaders.at("quad").handle, "mirror_v_on");
  int mirror_h_check = glGetUniformLocation(m_shaders.at("quad").handle, "mirror_h_on");
  int blur_check = glGetUniformLocation(m_shaders.at("quad").handle, "blur_on"); */

  glUniform1i(screentex_location, 0);
  //glUniform1b(inverse_check, inverse);
  //std::cout << inverse << std::endl;
  //glUniform1b(grayscale, grayscale);

  glBindVertexArray(quad_object.vertex_AO);
  glDrawArrays(quad_object.draw_mode, 0, quad_object.num_elements); //draw quad made out of two triangles 

  //glUseProgram(m_shaders.at("quad").handle);
  //int inverse_location = glGetUniformLocation(m_shaders.at("quad").handle, "inverse");
  //glUniform1i(inverse_location, inverse);
} 

////////////////////////////////////////////////////////////////////////////////////////////

void ApplicationSolar::skyboxrenderer(){
  glDepthMask(GL_FALSE);

  glUseProgram(m_shaders.at("skybox").handle);
  int skybox_location = glGetUniformLocation(m_shaders.at("skybox").handle, "skybox_location");
  glUniform1i(skybox_location, m_skytextures);
  glActiveTexture(GL_TEXTURE10);

  glBindTexture(GL_TEXTURE_CUBE_MAP, m_skytextures);
  glBindVertexArray(skybox_object.vertex_AO);
    
  glDrawArrays(skybox_object.draw_mode, 0,skybox_object.num_elements);
  //debugPrint("num elements: " + skybox_object.num_elements);
  glDepthMask(GL_TRUE);

}


void ApplicationSolar::orbitRenderer() const{
  glUseProgram(m_shaders.at("orbit").handle);
  //this Vector holds the values for which the orbits need to be scaled

  for(auto x : scene_graph_.getRoot().getChildrenList()){
    //debugPrint(x->getName());
    //debugPrint(x->getName());
    auto orbit = x->getChild("geo_" + x->getName() + "_orbit");
    if(orbit == nullptr){
      continue;
    }
    //std::cout << glm::to_string(x->getLocalTransform()) << std::endl;
    std::shared_ptr<Geometry_node> orbit_cast_ptr = std::static_pointer_cast<Geometry_node>(orbit);

    //here we scale the orbits
    glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ModelMatrix"),
                          1, GL_FALSE, glm::value_ptr((orbit->getLocalTransform())/*  * glm::scale({}, scaling_values[counter]) */)); 
    glBindBuffer(GL_ARRAY_BUFFER, orbit_object.vertex_BO);            
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* orbit_cast_ptr->getGeometry().data.size(), orbit_cast_ptr->getGeometry().data.data(), GL_STATIC_DRAW);
    glBindVertexArray(orbit_object.vertex_AO);
    glDrawArrays(orbit_object.draw_mode, GLint(0), orbit_object.num_elements);
    //}
    }
}

void ApplicationSolar::starRenderer() const{
    glUseProgram(m_shaders.at("star").handle);
    glBindVertexArray(star_object.vertex_AO);
    glDrawArrays(star_object.draw_mode,GLint(0), star_object.num_elements);
    //glDrawElements(star_object.draw_mode, star_object.num_elements, model::INDEX.type, &star_object);
 }


void ApplicationSolar::planetrenderer(){

  //render sun
  glUseProgram(m_shaders.at("sun").handle);
  glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 0.0f, 1.0f});
  model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, 0.0f});
  model_matrix = glm::scale(model_matrix, glm::fvec3{5.0f, 5.0f, 5.0f});
  glUniformMatrix4fv(m_shaders.at("sun").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  glUniformMatrix4fv(m_shaders.at("sun").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));

  
  int sampler_sun_location = glGetUniformLocation(m_shaders.at("sun").handle, "YourTexture");

  glBindVertexArray(planet_object.vertex_AO);
  glActiveTexture(GL_TEXTURE10);
  glBindTexture(GL_TEXTURE_2D, m_sunTexture); 
  glUniform1i(sampler_sun_location, 10);

  //debugPrint(std::to_string(x->getTexInt()));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);
/*     glBindVertexArray(planet_object.vertex_AO);
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);  */
  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);


  std::vector<std::shared_ptr<Node>> List_of_Planets;
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Earth"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Neptune"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Mars"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Venus"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Saturn"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Moon"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Jupiter"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Uranus"));
  List_of_Planets.push_back(scene_graph_.getRoot().getChild("Mercury"));

  glUseProgram(m_shaders.at("planet").handle);

  std::shared_ptr<Point_light_node> Schimmer = std::static_pointer_cast<Point_light_node>(scene_graph_.getRoot().getChild("Schimmer"));
  int camera_location = glGetUniformLocation(m_shaders.at("planet").handle, "pass_Camera");
  int planet_shader_location = glGetUniformLocation(m_shaders.at("planet").handle, "planet_colour");
  int light_shader_location = glGetUniformLocation(m_shaders.at("planet").handle, "light_colour");
  int light_intensity_shader_location = glGetUniformLocation(m_shaders.at("planet").handle, "light_intensity");
  int sampler_location = glGetUniformLocation(m_shaders.at("planet").handle, "YourTexture");
  int switch_app_bool = glGetUniformLocation(m_shaders.at("planet").handle, "switch_appearance");
  int mapping_location = glGetUniformLocation(m_shaders.at("planet").handle, "MappingTex");
  int using_mapping = glGetUniformLocation(m_shaders.at("planet").handle, "use_mapping");
  
  glm::vec4 cam_pos = scene_graph_.getRoot().getChild("Camera")->getLocalTransform()* m_view_transform *glm::vec4{0.0f,0.0f,0.0f,1.0f};
  glUniform3f(camera_location, cam_pos.x, cam_pos.y, cam_pos.z);
  glUniform1f(light_intensity_shader_location, Schimmer->getLightIntesity());
  glUniform3f(light_shader_location, Schimmer->getLightColour().x, Schimmer->getLightColour().y, Schimmer->getLightColour().z);
  glUniform1f(switch_app_bool, switch_appearence);

  int tmp = 10;
  unsigned int planet = 1;

  for(std::shared_ptr<Node> x : List_of_Planets){
    
    //if it doesent work maybe here an if statement
    //glUniform1i(mapping_location, x->getMappingInt());
    glUseProgram(m_shaders.at("planet").handle);

    glBindVertexArray(planet_object.vertex_AO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, x->getTexInt()); 

    glUniform1i(sampler_location, 0);
    glUniform3f(planet_shader_location, x->getColour().x, x->getColour().y, x->getColour().z);

    glActiveTexture(GL_TEXTURE1);

    if(x->is_using_mapping()){
      glBindTexture(GL_TEXTURE_2D, x->getMappingInt());
    }

    glUniform1i(mapping_location, 1);
    glUniform1b(using_mapping, x->is_using_mapping());

    glm::fmat4 final_matrix;

    glm::fmat4 rotation_around_self = glm::rotate(glm::fmat4{}, 0.0005f*tmp*speed ,glm::fvec3{0.0f, 1.0f, 0.0f});
    if(x->getName() == "Moon"){
      //getting the Geometry-node equivalent to Node x
      std::shared_ptr<Node> planet_geo = x->getChild("geo_" + x->getName());
      //initializes Matrix with localTransform of Parent of x
      //We set our orientation source to the local transform of the parent because we want our planets to rotate around their parent
      glm::fmat4 rotation_matrix = glm::rotate(glm::fmat4{}, 0.0001f*tmp*speed ,glm::fvec3{0.0f, 1.0f, 0.0f});
      x->getChild("geo_Moon")->getLocalTransform()*rotate(glm::fmat4{}, float(glfwGetTime()) ,glm::fvec3{0.0f, 1.0f, 0.0f});
      //we multiply LocalTransform of the Geometry Node and the rotation Matrix and set it as their parents localTransform,
      //this way x sees the parent as the center of the orbit
      glm::fmat4 newTransform = rotation_matrix * planet_geo->getParent()->getLocalTransform()*rotation_around_self; 
      planet_geo->getParent()->setLocalTransform(newTransform);

      final_matrix = earth_local_transform * planet_geo->getParent()->getLocalTransform() * planet_geo->getLocalTransform();
    }
    else{
      //getting the Geometry-node equivalent to Node x
      std::shared_ptr<Node> planet_geo = x->getChild("geo_" + x->getName());
      //initializes Matrix with localTransform of Parent of x
      //We set our orientation source to the local transform of the parent because we want our planets to rotate around their parent
      glm::fmat4 rotation_matrix = glm::rotate(glm::fmat4{}, 0.0001f*tmp*speed ,glm::fvec3{0.0f, 1.0f, 0.0f});
      
      //we multiply LocalTransform of the Geometry Node and the rotation Matrix and set it as their parents localTransform,
      //this way x sees the parent as the center of the orbit
      glm::fmat4 newTransform = rotation_matrix * planet_geo->getParent()->getLocalTransform()*rotation_around_self; 
      planet_geo->getParent()->setLocalTransform(newTransform);
      //debugPrint(glm::to_string(planet_geo->getParent()->getLocalTransform()));
      final_matrix = planet_geo->getWorldTransform();
      //debugPrint(glm::to_string(planet_geo->getLocalTransform()));
      if(x->getName() == "Earth"){
        earth_local_transform = newTransform;
        //debugPrint("earth " + glm::to_string(final_matrix));
      }
      //debugPrint(glm::to_string(final_matrix));
    }

    tmp++;
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(final_matrix));
    final_matrix = glm::inverseTranspose(final_matrix);

    glEnableVertexArrayAttrib(planet_object.vertex_AO,0);

    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
    //planet_geo->getParent()->setLocalTransform(Local_storage);
    planet++;


  }
}



////////////////////////////////////////////////////////////////////////////////////////////

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);

  glUseProgram(m_shaders.at("orbit").handle);
  glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ViewMatrix"),
                      1, GL_FALSE, glm::value_ptr(view_matrix));

  glUseProgram(m_shaders.at("star").handle);
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ModelViewMatrix"),
                      1, GL_FALSE, glm::value_ptr(view_matrix)); 


  glUseProgram(m_shaders.at("planet").handle);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix)); 

  glUseProgram(m_shaders.at("sun").handle);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("sun").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix)); 

  glUseProgram(m_shaders.at("skybox").handle);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("skybox").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix)); 

  glUseProgram(m_shaders.at("quad").handle);
  // upload bool to quad shader
  glUniform1i(m_shaders.at("quad").u_locs.at("inverse"), inverse);
  glUniform1i(m_shaders.at("quad").u_locs.at("grayscale_on"), grayscale);
  glUniform1i(m_shaders.at("quad").u_locs.at("mirror_h_on"), mirror_h);
  glUniform1i(m_shaders.at("quad").u_locs.at("mirror_v_on"), mirror_v);
  glUniform1i(m_shaders.at("quad").u_locs.at("blur_on"), blur);
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
  glUseProgram(m_shaders.at("sun").handle);
  glUniformMatrix4fv(m_shaders.at("sun").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
  glUseProgram(m_shaders.at("star").handle);
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ProjectionMatrix"),
                      1, GL_FALSE, glm::value_ptr(m_view_projection));
  glUseProgram(m_shaders.at("orbit").handle);
  glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ProjectionMatrix"),
                      1, GL_FALSE, glm::value_ptr(m_view_projection));  
  glUseProgram(m_shaders.at("skybox").handle);
  glUniformMatrix4fv(m_shaders.at("skybox").u_locs.at("ProjectionMatrix"),
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

  m_shaders.emplace("sun", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/sun.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/sun.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("sun").u_locs["NormalMatrix"] = -1;
  m_shaders.at("sun").u_locs["ModelMatrix"] = -1;
  m_shaders.at("sun").u_locs["ViewMatrix"] = -1;
  m_shaders.at("sun").u_locs["ProjectionMatrix"] = -1;

  m_shaders.emplace("star", shader_program{{{GL_VERTEX_SHADER, m_resource_path + "shaders/vao.vert"},
                                            {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}}});
  m_shaders.at("star").u_locs["ModelViewMatrix"] = -1;
  m_shaders.at("star").u_locs["ProjectionMatrix"] = -1;

  m_shaders.emplace("skybox", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/skybox.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/skybox.frag"}}});
  m_shaders.at("skybox").u_locs["ViewMatrix"] = -1;
  m_shaders.at("skybox").u_locs["ProjectionMatrix"] = -1;
  m_shaders.at("star").u_locs["ModelViewMatrix"] = -1;
  m_shaders.at("sun").u_locs["NormalMatrix"] = -1;

  m_shaders.emplace("orbit", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/orbit.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/orbit.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("orbit").u_locs["ModelMatrix"] = -1;
  m_shaders.at("orbit").u_locs["ViewMatrix"] = -1;
  m_shaders.at("orbit").u_locs["ProjectionMatrix"] = -1;

  m_shaders.emplace("quad", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/quad.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/quad.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("quad").u_locs["grayscale_on"] = 0;
  m_shaders.at("quad").u_locs["mirror_v_on"] = 0;
  m_shaders.at("quad").u_locs["mirror_h_on"] = 0;
  m_shaders.at("quad").u_locs["blur_on"] = 0;
  m_shaders.at("quad").u_locs["inverse"] = 0;

  //std::cout << "initialize shader programms " << std::endl;

}

// load models
void ApplicationSolar::initializeGeometry() {

  //initialise quad object

  float quadVertices[] = {
    //pos          //texCoords
    -1.0f, 1.0f,   0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,   1.0f, 0.0f,

    -1.0f, 1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f, 1.0f,  1.0f, 1.0f
  };

  // generate vertex array object
  glGenVertexArrays(1, &quad_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(quad_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &quad_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, quad_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  4 * sizeof(float), (void*)(2*sizeof(float)));
  
   // store type of primitive to draw
  quad_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  quad_object.num_elements = GLsizei(6);

  model planet_model = model_loader::obj(m_resource_path + "models/sphere1.obj", model::NORMAL | model::TEXCOORD);

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
  
  //for the Textures
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::TEXCOORD]);


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
  //std::cout << "Geometry_initializer" << std::endl;
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.5f});
    uploadView();
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.5f});
    uploadView();
  }
  else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.5f, 0.0f, 0.0f});
    uploadView();
  }
  else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.5f, 0.0f, 0.0f});
    uploadView();
  }
//---------------------------------------------------------------------------------------------------------
  else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    speed = speed-0.1f;
    uploadView();
  }
  else if (key == GLFW_KEY_P && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    speed = 0.0f;
    uploadView();
  }
/*   else if (key == GLFW_KEY_P && (action == GLFW_RELEASE || action == GLFW_REPEAT)) {
    speed = 3.0f;
    uploadView();
  } */
  else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    speed = speed+0.1f;
    uploadView();
  }
//---------------------------------------------------------------------------------------------------------

  else if (key == GLFW_KEY_1 && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    switch_appearence = false;
    uploadView();
  }
  else if (key == GLFW_KEY_2 && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    switch_appearence = true;
    uploadView();
  }
  else if(key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    inverse = !inverse;
    uploadView();
  }
  else if(key == GLFW_KEY_7 && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    grayscale = !grayscale;
    uploadView();
  }
  else if(key == GLFW_KEY_8 && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    std::cout << mirror_h << std::endl;
    mirror_h = !mirror_h;
    uploadView();
  }
  else if(key == GLFW_KEY_9 && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    std::cout << mirror_v << std::endl;
    mirror_v = !mirror_v;
    uploadView();
  }
  else if(key == GLFW_KEY_0 && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    blur = !blur;
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
  std::cout<< "width: "<< width << std::endl;
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
  initializeFramebuffer(width, height);
  m_width = width;
  m_height = height;
}


// exe entry point
int main(int argc, char* argv[]) {

  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}
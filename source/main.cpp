#include <stdio.h>
#include <iostream>
#include <cstring>



#include "window.h"
#include "gfx/gfx.h"
#include "gfx/gfx_utils.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utils/mesh_loader.h"
#include "utils/stb_perlin.h"
#include "utils/mesh_generator.h"

#include "renderer/camera.h"
#include "renderer/mesh.h"
#include "renderer/light.h"

#include "gui/gui.h"


float vertices[] = {
    1.f, 1.f, 0.0f, 0.0f, 1.0f,   // top right
    1.f, -1.f, 0.0f, 0.0f, 0.0f,  // bottom right
    -1.f, -1.f, 0.0f, 1.0f, 0.0f, // bottom left
    -1.f, 1.f, 0.0f, 1.0f, 1.0f   // top left
};

u32 indices[] = {0, 1, 2, 0, 2, 3};

float dt = 0.f;
renderer::free_camera_t camera(90, 640.f / 480.f, 0.001f, 100000.f);

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);



int main()
{
  window_t window("gl_renderer", 640, 480);
  gfx::init();

  gfx::program_t p_default = gfx_utils::program_load("./res/shaders/default.vs", "./res/shaders/default.fs");
  gfx::program_t p_perlin_mesh = gfx_utils::program_load("./res/shaders/perlin_mesh.vs", "./res/shaders/perlin_mesh.fs");
  gfx::program_t p_light = gfx_utils::program_load("./res/shaders/basic_light.vs", "./res/shaders/basic_light.fs");
  gfx::program_t p_skybox  = gfx_utils::program_load("./res/shaders/skybox.vs", "./res/shaders/skybox.fs");


  gfx::vertex_array_t vao = gfx::vertex_array_ctor();
  // so i am not sure about this design
  // maybe i will change it later to something else
  // that requieres less number of lines
  gfx::bind_vertex_array(vao);



  //glfwSetInputMode(window.window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window.window_, mouse_callback);

  float last_frame = 0.0f;

  camera.position_ = {-0.15, -0.15, -5};


  auto skybox_mesh = obj_load_mesh("./res/cube.obj");
  auto mesh = obj_load_mesh("./res/cube.obj");

  const char* cubemap_path[] = {
    "./res/skybox/skybox/right.jpg",
    "./res/skybox/skybox/left.jpg",
    "./res/skybox/skybox/top.jpg",
    "./res/skybox/skybox/bottom.jpg",
    "./res/skybox/skybox/front.jpg",
    "./res/skybox/skybox/back.jpg"
  } ;

  auto cube_map = gfx_utils::texture_load_cubemap(cubemap_path);

  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  // f32 octaves = 8.0f;
  // f32 lacunarity = 2.0f;
  // f32 gain = 0.5f;
  // double scale = 160.0;
  // double scale_w = 3.0;
  // double heightmap[128][128] = {0.0};
  // glm::vec2 dim = glm::vec2(128, 128);

  // srand(time(NULL));
  // int seed = rand();

  
  // f32 min, max = 0.0;
  // for(i32 i = 0; i < dim.x; i++){
  //   for(i32 j = 0; j < dim.y; j++){
  //     heightmap[i][j] = stb_perlin_fbm_noise3(i*(1.0/dim.x), j*(1.0/dim.y), 0, lacunarity, gain, octaves);
  //     if(heightmap[i][j] > max) max = heightmap[i][j];
  //     if(heightmap[i][j] < min) min = heightmap[i][j];
  //   }
  // }

  // //Normalize
  // for(i32 i = 0; i < dim.x; i++){
  //   for(i32 j = 0; j < dim.y; j++){
  //     heightmap[i][j] = (heightmap[i][j] - min)/(max - min);
  //   }
  // }

  // std::vector<u32> indices;
  // std::vector<f32> positions;


  // //Loop over all positions and add the triangles!
  // for(i32 i = 0; i < dim.x-1; i++){
  //   for(i32 j = 0; j < dim.y-1; j++){

  //     //Add to Position Vector
  //     glm::vec3 a = glm::vec3(scale_w*i, scale*heightmap[i][j], scale_w*j);
  //     glm::vec3 b = glm::vec3(scale_w*(i+1), scale*heightmap[i+1][j], scale_w*j);
  //     glm::vec3 c = glm::vec3(scale_w*i, scale*heightmap[i][j+1], scale_w*(j+1));
  //     glm::vec3 d = glm::vec3(scale_w*(i+1), scale*heightmap[i+1][j+1], scale_w*(j+1));

  //     //UPPER TRIANGLE

  //     //Add Indices
  //     indices.push_back(positions.size()/3+0);
  //     indices.push_back(positions.size()/3+1);
  //     indices.push_back(positions.size()/3+2);
  //     indices.push_back(positions.size()/3+0);

  //     add(positions, a);
  //     add(positions, b);
  //     add(positions, c);


  //     //Lower Triangle
  //     indices.push_back(positions.size()/3+0);
  //     indices.push_back(positions.size()/3+1);
  //     indices.push_back(positions.size()/3+2);
  //     indices.push_back(positions.size()/3+0);

  //     add(positions, d);
  //     add(positions, c);
  //     add(positions, b);


  //   }
  // }

 

  // renderer::mesh_t perlin_mesh;
  // {
  //   gfx::buffer_t vbo = gfx::buffer_ctor((gfx::buffer_desc_t){
  //     positions.size()*sizeof(f32),
  //     positions.data(),
  //     gfx::vertex_buffer
  //   });
  //   gfx::buffer_t ibo = gfx::buffer_ctor((gfx::buffer_desc_t){
  //     indices.size()*sizeof(u32),
  //     indices.data(),
  //     gfx::index_buffer
  //   });
  //   perlin_mesh = renderer::mesh_t("", vbo, ibo, indices.size());
  // }

  renderer::mesh_t perlin_mesh = generate_terrain_mesh(128, 128, {3, 150, 3});

  gui::init(window.window_);

  renderer::light_t light_red({5,0,0}, {0.2, 0, 0}, 0.06f, 1.f, 512);

    glm::mat4 model1(1.f);
    glm::mat4 model2 = glm::translate(glm::mat4(1.f), {-5, 0, 0});


  while (1)
  {

    processInput(window.window_);
    gfx::clear_color(0.25f, 0.25f, 0.25f);
    gfx::clear(1);

    float current_frame = glfwGetTime();
    dt = current_frame - last_frame;
    last_frame = current_frame;

    camera.update_view();
    glm::mat4 viewproj = camera.get_viewprojection();

     
   
glDepthMask(GL_FALSE);
    gfx::bind_buffer(skybox_mesh.vertex_buffer_);
    gfx::bind_buffer(skybox_mesh.index_buffer_);
    gfx::vertex_array_set_layout({{gfx::gl_float3},
                                  {gfx::gl_float2},
                                  {gfx::gl_float3}});
    gfx::bind_program(p_skybox);
    gfx::bind_texture(cube_map, 0);
    gfx::set_uniform_mat4(p_skybox, "u_view", glm::mat4(glm::mat3(camera.view_)));
    gfx::set_uniform_mat4(p_skybox, "u_proj", camera.projection_);
    gfx::set_uniform_int(p_skybox, "skybox", 0);
    gfx::draw_elements(gfx::gl_triangles, skybox_mesh.index_count_, gfx::gl_uint, 0);
glDepthMask(GL_TRUE);

    gfx::bind_buffer(mesh.vertex_buffer_);
    gfx::bind_buffer(mesh.index_buffer_);
    gfx::vertex_array_set_layout({{gfx::gl_float3}, {gfx::gl_float2}, {gfx::gl_float3}});
    gfx::bind_program(p_light);
    gfx::set_uniform_mat4(p_light, "u_view", camera.view_);
    gfx::set_uniform_mat4(p_light, "u_proj", camera.projection_);
    gfx::set_uniform_mat4(p_light, "u_model", model1);
    gfx::set_uniform_vec3(p_light, "u_light_pos", light_red.position_);
    gfx::set_uniform_vec3(p_light, "u_view_pos", camera.position_);
    gfx::set_uniform_vec3(p_light, "u_light_color", light_red.color_);
    gfx::set_uniform_vec3(p_light, "u_object_color", {1, 1, 1});
    gfx::set_uniform_float(p_light, "u_ambient_strength", light_red.ambient_strength_);
    gfx::set_uniform_float(p_light, "u_specular_strength", light_red.specular_strength_);
    gfx::set_uniform_int(p_light, "u_resolution", light_red.resolution_);
    gfx::draw_elements(gfx::gl_triangles, mesh.index_count_, gfx::gl_uint, 0);

    gfx::bind_buffer(perlin_mesh.vertex_buffer_);
    gfx::bind_buffer(perlin_mesh.index_buffer_);
    gfx::vertex_array_set_layout({{gfx::gl_float3}});
    gfx::bind_program(p_perlin_mesh);
    gfx::set_uniform_mat4(p_perlin_mesh, "u_view", camera.view_);
    gfx::set_uniform_mat4(p_perlin_mesh, "u_proj", camera.projection_);
    gfx::set_uniform_mat4(p_perlin_mesh, "u_model", model2);
    gfx::draw_elements(gfx::gl_lines, perlin_mesh.index_count_, gfx::gl_uint, 0);

    gui::begin();
      static i32 terrain_size[2] = {3,3};
      static glm::vec3 terrain_scale = {1,1,1};
      static f32 octaves = 1.f, lacunarity = 1.f, gain = 1.f;
      if (ImGui::Begin("Terrain"))
      {
        if(ImGui::Button("Generate"))
        {
          std::cout << "Clicl\n";
          perlin_mesh = generate_terrain_mesh(terrain_size[0], terrain_size[1], terrain_scale, octaves, lacunarity, gain);
        }
        ImGui::InputInt2("W/H", terrain_size);
        ImGui::DragFloat3("Scale", glm::value_ptr(terrain_scale), 1, 300);
        ImGui::InputFloat("Octaves", &octaves);
        ImGui::InputFloat("Lacunarity", &lacunarity);
        ImGui::InputFloat("Gain", &gain);

        ImGui::End();
      }



    gui::end();
   
    window.update();
    //std::cout << camera.position_.x << " " << camera.position_.y << " " << camera.position_.z << "\n";
  }
  return 0;
}

void processInput(GLFWwindow *window)
{
  glm::vec3 &camera_front = camera.camera_front_;
  glm::vec3 &camera_up = camera.camera_up_;
  glm::vec3 &camera_pos = camera.position_;
  const float camera_speed = 5.05f; // adjust accordingly
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera_pos += camera_speed * camera_front;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera_pos -= camera_speed * camera_front;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
}

float lastX = 400, lastY = 300;
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  float &yaw = camera.rotation_.x;
  float &pitch = camera.rotation_.y;
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
  lastX = xpos;
  lastY = ypos;

  const float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;
}
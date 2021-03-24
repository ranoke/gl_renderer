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

#include "renderer/camera.h"
#include "utils/mesh_loader.h"

#include "gui/gui.h"

std::string vertex_src = R"(
#version 330 core
layout (location = 0) in vec3 a_pos; // the position variable has attribute position 0
layout (location = 1) in vec2 a_uv;

uniform mat4 u_viewproj;
uniform mat4 u_model;

out vec2 uv;

void main()
{
  gl_Position = u_viewproj*u_model*vec4(a_pos, 1.0); // see how we directly give a vec3 to vec4's constructor
  uv = a_uv;
}
)";

std::string fragment_src = R"(
#version 330 core
out vec4 frag_color;

uniform sampler2D u_tex;
in vec2 uv;

void main()
{
  frag_color = texture(u_tex, uv);
} 
)";

std::string obj_vertex_src = R"(
#version 330 core
layout (location = 0) in vec3 a_pos; // the position variable has attribute position 0
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;

out vec2 uv;

void main()
{
  gl_Position = u_proj*u_view*vec4(a_pos, 1.0); // see how we directly give a vec3 to vec4's constructor
  uv = a_uv;
}
)";
std::string obj_fragment_src = R"(
#version 330 core
out vec4 frag_color;

uniform sampler2D u_tex;
in vec2 uv;
void main()
{
  frag_color = vec4(1, 0, 0, 1); 
}
)";


std::string skybox_vertex_src = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

out vec3 TexCoords;


uniform mat4 u_view;
uniform mat4 u_proj;
void main()
{
    TexCoords = aPos;
    gl_Position = u_proj * u_view * vec4(aPos, 1.0);
}  

)";

std::string skybox_fragment_src = R"(
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}
)";

float vertices[] = {
    1.f, 1.f, 0.0f, 0.0f, 1.0f,   // top right
    1.f, -1.f, 0.0f, 0.0f, 0.0f,  // bottom right
    -1.f, -1.f, 0.0f, 1.0f, 0.0f, // bottom left
    -1.f, 1.f, 0.0f, 1.0f, 1.0f   // top left
};

u32 indices[] = {0, 1, 2, 0, 2, 3};

float dt = 0.f;

renderer::free_camera_t camera(90, 640.f / 480.f, 0.001f, 1000.f);

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

int main()
{
  window_t window("gl_renderer", 640, 480);
  gfx::init();

  gfx::program_t p = gfx::program_ctor({gfx::shader_ctor(vertex_src, GL_VERTEX_SHADER),
                                        gfx::shader_ctor(fragment_src, GL_FRAGMENT_SHADER)});
  gfx::program_t p_default = gfx::program_ctor({gfx::shader_ctor(obj_vertex_src, GL_VERTEX_SHADER),
                                            gfx::shader_ctor(obj_fragment_src, GL_FRAGMENT_SHADER)});
  gfx::program_t p_skybox = gfx::program_ctor({gfx::shader_ctor(skybox_vertex_src, GL_VERTEX_SHADER),
                                            gfx::shader_ctor(skybox_fragment_src, GL_FRAGMENT_SHADER)});

  gfx::vertex_array_t vao = gfx::vertex_array_ctor();
  // so i am not sure about this design
  // maybe i will change it later to something else
  // that requieres less number of lines
  gfx::bind_vertex_array(vao);



  glfwSetInputMode(window.window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window.window_, mouse_callback);

  float last_frame = 0.0f;

  camera.position_ = {-0.15, -0.15, -5};

  glm::mat4 model(1.f);

  auto skybox_mesh = obj_load_mesh("./res/cube.obj");
  auto mesh = obj_load_mesh("./res/hr_sphere.obj");

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

  
 

  gui::init(window.window_);

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
    gfx::bind_program(p_default);
    gfx::set_uniform_mat4(p_default, "u_view", camera.view_);
    gfx::set_uniform_mat4(p_default, "u_proj", camera.projection_);
    gfx::draw_elements(gfx::gl_triangles, mesh.index_count_, gfx::gl_uint, 0);


    gui::begin();
      if (ImGui::Begin("Hello"))
      {
        ImGui::Button("Button");
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
  const float camera_speed = 0.05f; // adjust accordingly
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
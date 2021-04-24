#include <stdio.h>
#include <iostream>
#include <cstring>

#include "base/logger.h"

#include "base/base.h"

#include "window.h"
#include "gfx/gfx.h"
#include "gfx/gfx_utils.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utils/object_loader.h"
#include "utils/object_generator.h"
#include "utils/stb_perlin.h"

#include "renderer/camera.h"
#include "renderer/light.h"
#include "renderer/render_object.h"

#include "renderer/mesh.h"

#include "gui/gui.h"


gfx_utils::library_t<gfx::program_t> shader_library;
gfx_utils::library_t<gfx::texture_t> texture_library;


float last_frame = 0.0f;
float dt = 0.f; // delta time
renderer::free_camera_t camera(90, 640.f / 480.f, 0.001f, 10000.f);

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

float camera_speed = 0.5f; 
int main()
{
  logger::init();
  window_t window("gl_renderer", 800, 600);
  gfx::init();

  shader_library.add("defualt", gfx_utils::program_load("./res/shaders/default.vs", "./res/shaders/default.fs"));
  shader_library.add("perlin_terrain", gfx_utils::program_load("./res/shaders/perlin_mesh.vs", "./res/shaders/perlin_mesh.fs"));
  shader_library.add("basic_light", gfx_utils::program_load("./res/shaders/basic_light.vs", "./res/shaders/basic_light.fs"));
  shader_library.add("basic_material", gfx_utils::program_load("./res/shaders/materials.vs", "./res/shaders/materials.fs"));
  shader_library.add("skybox", gfx_utils::program_load("./res/shaders/skybox.vs", "./res/shaders/skybox.fs"));
  shader_library.add("grid", gfx_utils::program_load("./res/shaders/grid.vs", "./res/shaders/grid.fs"));
  shader_library.add("mesh", gfx_utils::program_load("./res/shaders/mesh.vs", "./res/shaders/mesh.fs"));
  shader_library.add("hdri_skybox", gfx_utils::program_load("./res/shaders/hdri_skybox.vs", "./res/shaders/hdri_skybox.fs"));

  const gfx::program_t& p_default = shader_library.get("default");
  const gfx::program_t& p_perlin_terrain = shader_library.get("perlin_terrain");
  const gfx::program_t& p_basic_light = shader_library.get("basic_light");
  const gfx::program_t& p_basic_material = shader_library.get("basic_material");
  const gfx::program_t& p_skybox = shader_library.get("skybox");
  const gfx::program_t& p_grid = shader_library.get("grid");
  const gfx::program_t& p_mesh = shader_library.get("mesh");
  const gfx::program_t& p_hdri_skybox = shader_library.get("hdri_skybox");
	
	texture_library.add("image",gfx_utils::texture_load("./res/image.jpg"));
	const auto& image = texture_library.get("image");

  auto night = gfx_utils::texture_load_hdri("./res/skybox/night.hdr");


  gfx::vertex_array_t vao = gfx::vertex_array_ctor();
  //// so i am not sure about this design
  //// maybe i will change it later to something else
  //// that requieres less number of lines
  gfx::bind_vertex_array(vao);

  glfwSetCursorPosCallback(window.window_, mouse_callback);


  renderer::render_object_t perlin_mesh = generate_terrain(128, 128, { 3, 150, 3 });
  renderer::render_object_t skybox_mesh = obj_load_render_object("./res/cube.obj");
  renderer::render_object_t mesh = obj_load_render_object("./res/cube.obj");
  renderer::render_object_t grid_mesh = obj_load_render_object("./res/grid.obj");


  const std::vector<const char*> cubemap_path = {
      "./res/skybox/skybox/right.jpg",
      "./res/skybox/skybox/left.jpg",
      "./res/skybox/skybox/top.jpg",
      "./res/skybox/skybox/bottom.jpg",
      "./res/skybox/skybox/front.jpg",
      "./res/skybox/skybox/back.jpg" };

  auto cube_map = gfx_utils::texture_load_cubemap(cubemap_path);

  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  gui::init(window.window_);

  renderer::light_t light_red({ 5, 0, 0 }, { 0.2, 0, 0 }, 0.06f, 1.f, 512);

  glm::mat4 model1(1.f);
  model1 = glm::scale(model1, { 10, 10, 10 });
  glm::mat4 model2 = glm::translate(glm::mat4(1.f), { -5, 0, 0 });
  model2 = glm::scale(model2, { 5, 5, 5 });
  glm::mat4 model_material = glm::translate(model1, { -10, 0, 0 });
  model_material = glm::scale(model_material, { 10, 10, 10 });
  glm::mat4 model_grid = glm::scale(model1, { 10, 10, 10 });

  //gfx::framebuffer_t fb = gfx::framebuffer_ctor({ 640, 480, gfx::framebuffer_attachment::framebuffer_color_attachment });

  
  renderer::mesh_t my_test_mesh = obj_load("./res/monkey_textures.obj");
  my_test_mesh.position_ = { 0, 200, 0 };
  my_test_mesh.scale_ = { 20, 20, 20 };
  while (!glfwWindowShouldClose(window.window_))
  {
		processInput(window.window_);
		gfx::clear_color(0.25f, 0.25f, 0.25f);
		gfx::clear(1);

		//gfx::bind_framebuffer(fb);
		glViewport(0, 0, 800, 600);
		f32 current_frame = static_cast<f32>(glfwGetTime());
		dt = current_frame - last_frame;
		last_frame = current_frame;

		camera.update_view();
		glm::mat4 viewproj = camera.get_viewprojection();
#define skybox
#define basic_light
#define grid
#define heightmap

#ifdef skybox
		glDepthMask(GL_FALSE);
		gfx::bind_buffer(skybox_mesh.vertex_buffer_);
		gfx::bind_buffer(skybox_mesh.index_buffer_);
		gfx::vertex_array_set_layout({ {gfx::gl_type::gl_float3},
																	 {gfx::gl_type::gl_float2},
																	 {gfx::gl_type::gl_float3} });
		gfx::bind_program(p_hdri_skybox);
		gfx::bind_texture(night, 0);
		gfx::set_uniform_mat4(p_hdri_skybox, "u_view", glm::mat4(glm::mat3(camera.view_)));
		gfx::set_uniform_mat4(p_hdri_skybox, "u_proj", camera.projection_);
		gfx::set_uniform_int(p_hdri_skybox, "skybox", 0);
		gfx::draw_elements(gfx::gl_draw_mode::triangles, skybox_mesh.index_count_, gfx::gl_type::gl_uint, 0);
		glDepthMask(GL_TRUE);
#endif
#ifdef heightmap
    gfx::bind_buffer(perlin_mesh.vertex_buffer_);
    gfx::bind_buffer(perlin_mesh.index_buffer_);
    gfx::vertex_array_set_layout({{gfx::gl_type::gl_float3}});
    gfx::bind_program(p_perlin_terrain);
    gfx::set_uniform_mat4(p_perlin_terrain, "u_view", camera.view_);
    gfx::set_uniform_mat4(p_perlin_terrain, "u_proj", camera.projection_);
    gfx::set_uniform_mat4(p_perlin_terrain, "u_model", model2);
    gfx::draw_elements(gfx::gl_draw_mode::lines, perlin_mesh.index_count_, gfx::gl_type::gl_uint, 0);
#endif

#ifdef basic_light
    gfx::bind_buffer(mesh.vertex_buffer_);
    gfx::bind_buffer(mesh.index_buffer_);
    gfx::vertex_array_set_layout({{gfx::gl_type::gl_float3}, {gfx::gl_type::gl_float2}, {gfx::gl_type::gl_float3}});
    gfx::bind_program(p_basic_light);
    gfx::set_uniform_mat4(p_basic_light, "u_view", camera.view_);
    gfx::set_uniform_mat4(p_basic_light, "u_proj", camera.projection_);
    gfx::set_uniform_mat4(p_basic_light, "u_model", model1);
    gfx::set_uniform_vec3(p_basic_light, "u_light_pos", light_red.position_);
    gfx::set_uniform_vec3(p_basic_light, "u_view_pos", camera.position_);
    gfx::set_uniform_vec3(p_basic_light, "u_light_color", light_red.color_);
    gfx::set_uniform_vec3(p_basic_light, "u_object_color", {1, 1, 1});
    gfx::set_uniform_float(p_basic_light, "u_ambient_strength", light_red.ambient_strength_);
    gfx::set_uniform_float(p_basic_light, "u_specular_strength", light_red.specular_strength_);
    gfx::set_uniform_int(p_basic_light, "u_resolution", light_red.resolution_);
    gfx::draw_elements(gfx::gl_draw_mode::triangles, mesh.index_count_, gfx::gl_type::gl_uint, 0);
#endif

    static glm::vec3 mat_ambient, mat_diffuse, mat_specular;
    static glm::vec3 light_ambient, light_diffuse, light_specular;
    static float shinnes = 32;

    gfx::bind_buffer(mesh.vertex_buffer_);
    gfx::bind_buffer(mesh.index_buffer_);
    gfx::vertex_array_set_layout({{gfx::gl_type::gl_float3}, {gfx::gl_type::gl_float2}, {gfx::gl_type::gl_float3}});
    gfx::bind_program(p_basic_material);
    gfx::set_uniform_mat4(p_basic_material, "u_view", camera.view_);
    gfx::set_uniform_mat4(p_basic_material, "u_proj", camera.projection_);
    gfx::set_uniform_mat4(p_basic_material, "u_model", model_material);
    gfx::set_uniform_vec3(p_basic_material, "u_view_pos", camera.position_);
    gfx::set_uniform_vec3(p_basic_material, "u_material.ambient", mat_ambient);
    gfx::set_uniform_vec3(p_basic_material, "u_material.diffuse", mat_diffuse);
    gfx::set_uniform_vec3(p_basic_material, "u_material.specular", mat_specular);
    gfx::set_uniform_float(p_basic_material, "u_material.shininess", shinnes);
    gfx::set_uniform_vec3(p_basic_material, "u_light.position", light_red.position_);
    gfx::set_uniform_vec3(p_basic_material, "u_light.ambient", light_ambient);
    gfx::set_uniform_vec3(p_basic_material, "u_light.diffuse", light_diffuse);
    gfx::set_uniform_vec3(p_basic_material, "u_light.specular", light_specular);

    gfx::draw_elements(gfx::gl_draw_mode::triangles, mesh.index_count_, gfx::gl_type::gl_uint, 0);

#ifdef grid
    gfx::bind_buffer(grid_mesh.vertex_buffer_);
    gfx::bind_buffer(grid_mesh.index_buffer_);
    gfx::vertex_array_set_layout({{gfx::gl_type::gl_float3}, {gfx::gl_type::gl_float2}, {gfx::gl_type::gl_float3}});
    gfx::bind_program(p_grid);
    gfx::set_uniform_mat4(p_grid, "u_view", camera.view_);
    gfx::set_uniform_mat4(p_grid, "u_proj", camera.projection_);
    gfx::set_uniform_mat4(p_grid, "u_model", model_grid);
    gfx::draw_elements(gfx::gl_draw_mode::line_strip, grid_mesh.index_count_, gfx::gl_type::gl_uint, 0);
#endif

    gfx::bind_texture(my_test_mesh.material_.bump_tex_, 0);
    my_test_mesh.render(p_mesh, camera);

    //gfx::bind_framebuffer({0});

    gui::begin();
    if (ImGui::Begin("Terrain"))
    {
      static i32 terrain_size[2] = {3, 3};
      static glm::vec3 terrain_scale = {1, 1, 1};
      static f32 octaves = 1.f, lacunarity = 1.f, gain = 1.f;
      if (ImGui::Button("Generate"))
      {
        perlin_mesh = generate_terrain(terrain_size[0], terrain_size[1], terrain_scale, octaves, lacunarity, gain);
      }
      ImGui::InputInt2("W/H", terrain_size);
      ImGui::DragFloat3("Scale", glm::value_ptr(terrain_scale), 1, 300);
      ImGui::InputFloat("Octaves", &octaves);
      ImGui::InputFloat("Lacunarity", &lacunarity);
      ImGui::InputFloat("Gain", &gain);

      ImGui::End();
    }

    if (ImGui::Begin("Settings"))
    {
      ImGui::DragFloat("Camera speed", &camera_speed);

      ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
      ImVec2 m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
      ImTextureID my_tex_id = (ImTextureID)(static_cast<u64>(night.texture_));
      ImGui::Image(my_tex_id, m_ViewportSize, ImVec2{0, 1}, ImVec2{1, 0});

      ImGui::End();
    }

    if (ImGui::Begin("Material"))
    {

      ImGui::ColorEdit3("M_ambient", glm::value_ptr(mat_ambient));
      ImGui::ColorEdit3("M_diffuse", glm::value_ptr(mat_diffuse));
      ImGui::ColorEdit3("M_specular", glm::value_ptr(mat_specular));
      ImGui::ColorEdit3("L_ambient", glm::value_ptr(light_ambient));
      ImGui::ColorEdit3("L_diffuse", glm::value_ptr(light_diffuse));
      ImGui::ColorEdit3("L_specular", glm::value_ptr(light_specular));
      ImGui::InputFloat("Shinnes", &shinnes);
      ImGui::End();
    }

    if (ImGui::Begin("Viewport"))
    {
      ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
      ImVec2 m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
      //ImTextureID my_tex_id = (ImTextureID)(static_cast<u64>(fb.texture_.texture_));
      //ImGui::Image(my_tex_id, m_ViewportSize, ImVec2{0, 1}, ImVec2{1, 0});

      ImGui::End();
    }

    gui::end();

    window.update();
    //std::cout << camera.position_.x << " " << camera.position_.y << " " << camera.position_.z << "\n";
  }
  //stb_leakcheck_dumpmem();
  return 0;
}

void processInput(GLFWwindow *window)
{
  glm::vec3 &camera_front = camera.camera_front_;
  glm::vec3 &camera_up = camera.camera_up_;
  glm::vec3 &camera_pos = camera.position_;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera_pos += camera_speed * camera_front;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera_pos -= camera_speed * camera_front;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

f32 lastX = 400, lastY = 300;
void mouse_callback(GLFWwindow *window, f64 xpos, f64 ypos)
{
  f32& yaw = camera.rotation_.x;
  f32& pitch = camera.rotation_.y;
  f32 xoffset = static_cast<f32>(xpos) - lastX;
  f32 yoffset = lastY - static_cast<f32>(ypos); // reversed since y-coordinates range from bottom to top
  lastX = static_cast<f32>(xpos);
  lastY = static_cast<f32>(ypos);

  const f32 sensitivity = 0.1;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;
}
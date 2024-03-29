#include "window.h"
#include "base/logger.h"

#include <cassert>
#include <GLFW/glfw3.h>

window_t::window_t(const std::string& title, u32 width, u32 height)
    : title_(std::string(title)), width_(width), height_(height)
{
  {
    int status = glfwInit();
    if(!status)
    {
      logger::error("glfwInit() failed with status {0}", status);
      assert(false);
      exit(-1);
    }
  }

#ifdef R_PLATFORM_LINUX 
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#elif R_PLATFORM_WINDOWS
#endif

  window_ = glfwCreateWindow(width, height, title.c_str(), 0, nullptr);
  if(!window_)
  {
    logger::error("Failed to create window");
    assert(false);
    exit(-1);
  }

  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);
}

void window_t::update()
{
  glfwPollEvents();
  glfwSwapBuffers(window_);
}

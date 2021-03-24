#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "base/types.h"

#include <string>


struct GLFWwindow;

struct window_t
{
  window_t(const std::string& title, u32 width, u32 height);
  void update();

  std::string title_;
  u32 width_;
  u32 height_;
  GLFWwindow* window_;
  
};


#endif // __WINDOW_H__

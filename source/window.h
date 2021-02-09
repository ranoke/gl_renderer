#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "types.h"

#include <string>


struct GLFWwindow;

class window_t
{
public:
  window_t(const std::string& title, u32 width, u32 height);
  void update();
private:
  std::string title_;
  u32 width_;
  u32 height_;
  GLFWwindow* window_;
  
};


#endif // __WINDOW_H__

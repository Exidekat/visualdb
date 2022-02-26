/* visualdb.h */
#pragma once

#include <iostream>
#include <map>
#include <cstdlib>
#include <array>

//glisms
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>

constexpr glm::vec4 rgba8_to_float(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
  return {r / 255.f, g / 255.f, b / 255.f, a / 255.f};
}

void update();
inline std::string query;  // Active VDB query.
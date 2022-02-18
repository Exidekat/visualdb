// visualdb.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <map>

//fontisms
#include <ft2build.h>
#include FT_FREETYPE_H

//glisms
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>

int fbw, fbh;

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexCode, const char* fragmentCode);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
};

void RenderText(Shader& s, const std::string& text, float x, float y, float scale, glm::vec3 color);

/* Map with char(glyphname) and a struct for conveniently storing glyph data */
struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

//glyph vertex shader
const char* g_vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>\n"
"out vec2 TexCoords;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
"   TexCoords = vertex.zw;\n"
"}\0";
//glyph fragment shader
const char* g_fragmentShaderSource = "#version 330 core\n"
"in vec2 TexCoords;\n"
"out vec4 color;\n"
"uniform sampler2D text;\n"
"uniform vec3 textColor;\n"
"void main()\n"
"{\n"
"   vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
"   color = vec4(textColor, 1.0) * sampled;\n"
"}\n\0"; // \n necessary?
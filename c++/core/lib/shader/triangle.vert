#version 330 core

layout (location = 0) in vec3 aPos;   // pVBO
layout (location = 1) in vec3 aColor; // cVBO
  
out vec3 ourColor; // send color to fragment shader
uniform mat4 translation;
uniform mat4 perspective;

void main()
{
    gl_Position = perspective * translation *   vec4(aPos, 1.0);
    ourColor = aColor;
}

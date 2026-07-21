#version 330 core

layout (location = 0) in vec3 aPos;   // pVBO 
layout (location = 1) in vec2 gPos;
  

uniform mat4 translation;
uniform mat4 perspective;
vec3 pos;
uniform bool meshvgrid;

void main()
{   
    if (meshvgrid)
    gl_Position = perspective * translation *   vec4(gPos.x, 0.0, gPos.y, 1.0);
    else
    gl_Position = perspective * translation *   vec4(aPos, 1.0);
}


#version 330 core

layout (location = 0) in vec3 aPos;   // pVBO 
layout (location = 1) in vec2 gPos;
  

uniform mat4 translation;
uniform mat4 perspective;
uniform mat4 orient;
uniform bool meshvgrid;

void main()
{   
    if (meshvgrid)
    gl_Position = perspective * translation * orient * vec4(gPos.x, gPos.y, 0.0, 1.0);
    else
    gl_Position = perspective * translation * orient * vec4(aPos, 1.0);
}


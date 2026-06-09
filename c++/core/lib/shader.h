#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    public:
        uint program_index;
        Shader(const char *vspath, const char *fspath)
        {
            std::string vscode;
            std::string fscode;
            std::ifstream vsfile;
            std::ifstream fsfile;
            
            vsfile.open(vspath);
            fsfile.open(fspath);
            std::stringstream vsread, fsread;
            vsread << vsfile.rdbuf();
            fsread << fsfile.rdbuf();
            vsfile.close();
            fsfile.close();
            std::string vertexSource = vsread.str();
            std::string fragmentSource = fsread.str();

            const char* vShaderCode = vertexSource.c_str();
            const char* fShaderCode = fragmentSource.c_str();
            uint vertex, fragment;
            
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            
            program_index = glCreateProgram();
            glAttachShader(program_index, vertex);
            glAttachShader(program_index, fragment);

            glLinkProgram(program_index);
            
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        };
        void Use()
        {
            glUseProgram(program_index);
        }
};
#endif

//
// Created by aviator on 5/2/16.
//
#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

class GameShader
{
public:
    GameShader(const char * vertex_file_path, const char * fragment_file_path);
//!------------------------------
    GLuint 	getProgram();
    void  	use();
private:
    GLuint programID;
};

#endif
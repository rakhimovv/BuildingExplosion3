//
// Created by aviator on 5/6/16.
//

#include <SFML/Graphics.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <SOIL/SOIL.h>
#include "SkyboxRenderer.h"
#include "WindowSize.h"

GLfloat cube_vertices[] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
};

SkyBoxRenderer::SkyBoxRenderer():
        cubeVertices(std::begin(cube_vertices), std::end(cube_vertices)),
        cubeMapShader("data/shaders/skyboxvertex.frag", "data/shaders/skyboxfragment.frag")
{
    projectionMatrix = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.01f, 500.0f);

    boxFaces.push_back("data/hills_rt.tga");
    boxFaces.push_back("data/hills_lf.tga");
    boxFaces.push_back("data/hills_up.tga");
    boxFaces.push_back("data/hills_dn.tga");
    boxFaces.push_back("data/hills_bk.tga");
    boxFaces.push_back("data/hills_ft.tga");

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindVertexArray(skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(GLfloat), &cubeVertices[0],
            GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindVertexArray(0);

    loadCubeMap();
}

void SkyBoxRenderer::render(Camera * camera)
{
    glDepthMask(GL_FALSE);
    cubeMapShader.use();
//    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
//    view = glm::rotate(view, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

    GLuint viewLoc = glGetUniformLocation(cubeMapShader.getProgram(), "view");
    GLuint modelLoc = glGetUniformLocation(cubeMapShader.getProgram(), "model");
    GLuint projectionLoc = glGetUniformLocation(cubeMapShader.getProgram(), "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glBindVertexArray(skyboxVAO);

    glActiveTexture(GL_TEXTURE0_ARB);
    glUniform1i(glGetUniformLocation(cubeMapShader.getProgram(), "skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
    glEnable(GL_TEXTURE_CUBE_MAP);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
}

void SkyBoxRenderer::loadCubeMap()
{
    glGenTextures(1, &cubeMapTexture);

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

    for(GLuint i = 0; i < boxFaces.size(); i++)
    {
        sf::Image image;
        if (!image.loadFromFile(boxFaces[i])) {
            std::cout << "Unable to load [" << boxFaces[i] << "]" << std::endl;
            return;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
                     image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

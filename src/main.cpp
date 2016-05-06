#define GLEW_STATIC
#define SFML_STATIC

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

// GLM должна быть в /usr/include
// причем последняя версия с сайта, т.к. здесь везде углы в радианах
// 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CubeRenderer.h"
#include "SkyboxRenderer.h"
#include "WindowSize.h"

#include "shader.h"


void drawCubes (sf::RenderWindow &window, sf::Clock &gameClock)
{
    //GameShader cubeShader("data/shaders/cubevertex.frag", "data/shaders/cubefragment.frag");

    // Инициалиазация openGl для примитивов в классах обернута
    SkyBoxRenderer skyBoxRenderer;
    CubeRenderer cubeRenderer;


    bool isRunning = true;
    while (isRunning) {
        sf::Event windowEvent;

        while (window.pollEvent(windowEvent)) {
            if (windowEvent.type == sf::Event::Closed ||
                (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Escape)) {
                isRunning = false;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skyBoxRenderer.render();

        for (size_t i = 0; i < 2; i++) {
            Cube cube(glm::vec3(-3.0f + 2*i + 0.5f, 0.0f, 0.0f), 1.0f); // 1 координата - центр, 2-ое - ребро TODO - quaternions
            cubeRenderer.render(cube); // один рендерер на все кубики
        }

        window.display();
    }

    window.close();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "OpenGL", sf::Style::Default, sf::ContextSettings(32, 8, 3, 3, 0));
    sf::Clock gameClock;

    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, window.getSize().x, window.getSize().y);
    glClearColor(1.0f, 0.1f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    drawCubes(window, gameClock);

    return 0;
}

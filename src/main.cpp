#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "WindowSize.h"
#include "GameObjects/GameSystem.h"


int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "BuildingExplosion3", sf::Style::Default,
                            sf::ContextSettings(32, 8, 3, 3, 0));

    sf::Clock gameClock;
    float elapsedTime = 0.0f;
    float lastUpdateTime = 0.0f;
    float constTimeStep = 1.0f / 30.0f;

    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, window.getSize().x, window.getSize().y);
    glClearColor(1.0f, 0.1f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    //GameShader cubeShader("data/shaders/cubevertex.frag", "data/shaders/cubefragment.frag");

    GameSystem gameSystem(constTimeStep);

    LineRenderer lineRenderer;

    bool isRunning = true;
    while (isRunning) {
        sf::Event windowEvent;

        while (window.pollEvent(windowEvent)) {
            if (windowEvent.type == sf::Event::Closed ||
                (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Escape)) {
                isRunning = false;
            }
        }

        float currTime = gameClock.getElapsedTime().asSeconds();
        if (currTime - lastUpdateTime > constTimeStep) {
            lastUpdateTime += constTimeStep;
            elapsedTime += constTimeStep;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            gameSystem.Update(constTimeStep);

            window.display();
        }
    }

    window.close();

    return 0;
}

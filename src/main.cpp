#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "WindowSize.h"
#include "GameObjects/GameSystem.h"
#include <queue>

#include "../dependencies/json/json.h"
#include "GameParameters.h"

int main()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT, desktop.bitsPerPixel), "BuildingExplosion3", sf::Style::Default,
                            sf::ContextSettings(32, 8, 4, 3, 0));

    sf::Clock gameClock;

    float elapsedTime = 0.0f;
    float lastUpdateTime = 0.0f;
    float constTimeStep = 1.0f / 60.0f;

    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, window.getSize().x, window.getSize().y);
    glClearColor(1.0f, 0.1f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    //GameShader cubeShader("data/shaders/cubevertex.frag", "data/shaders/cubefragment.frag");

    GameSystem gameSystem(constTimeStep);

//    GameParameters gameParameters("data/gameconfig.json");


    // Очередь для нажатых клавиш (т.к. у нас есть промежутки между апдейтами)
    std::queue<sf::Keyboard::Key> buttonsCache;

    bool isRunning = true;
    while (isRunning) {
        sf::Event windowEvent;

        while (window.pollEvent(windowEvent)) {
            switch(windowEvent.type) {
                case sf::Event::Closed:
                    isRunning = false;
                    break;
                case sf::Event::KeyPressed:
                    buttonsCache.push(windowEvent.key.code);
                    if (windowEvent.key.code == sf::Keyboard::Escape) {
                        isRunning = false;
                    }
                    break;
                default:
                    break;
            }
        }


        float currTime = gameClock.getElapsedTime().asSeconds();
        if (currTime - lastUpdateTime > constTimeStep) {
            lastUpdateTime += constTimeStep;
            elapsedTime += constTimeStep;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            gameSystem.Update(constTimeStep, buttonsCache);

            window.display();
        }
    }

    window.close();

    return 0;
}

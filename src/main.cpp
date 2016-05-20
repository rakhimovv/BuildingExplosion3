//#define GLEW_STATIC
//#define SFML_STATIC
//#define SFGUI_STATIC

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
//#include <SFGUI/SFGUI.hpp>
#include "WindowSize.h"
#include "GameObjects/GameSystem.h"
#include <queue>

#include "../dependencies/json/json.h"
#include "GameParameters.h"

int main()
{
    //sf::RenderWindow enterWindow(sf::VideoMode(WIDTH, HEIGHT), "BuildingExplosion3");
    //Menu(enterWindow);
    //enterWindow.close();

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "BuildingExplosion3", sf::Style::Default,
                            sf::ContextSettings(32, 8, 4, 2, 0));
//    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "BuildingExplosion3");
//    window.resetGLStates();

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

//    window.pushGLStates();
//    GameSystem gameSystem(constTimeStep);
//    window.popGLStates();

    // Очередь для нажатых клавиш (т.к. у нас есть промежутки между апдейтами)
    std::queue<sf::Keyboard::Key> buttonsCache;

    int menuNum = 0;
    sf::Texture menuTexture1, menuTexture2, menuBackground;
    if (!menuTexture1.loadFromFile("data/start.png")) {
        std::cout << "problem!\n";
    }
    if (!menuTexture2.loadFromFile("data/exit.png")) {
        std::cout << "problem1!\n";
    }
    if (!menuBackground.loadFromFile("data/background.jpg")) {
        std::cout << "problem2!\n";
    }

    sf::Sprite menu1(menuTexture1), menu2(menuTexture2), menuBg(menuBackground);
    menu1.setPosition(150, 200);
    menu2.setPosition(150, 300);
    menuBg.setPosition(0, 0);


    bool isMenuLive = true;
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

        window.clear();

        if (isMenuLive) {
            menu1.setColor(sf::Color::White);
            menu2.setColor(sf::Color::White);
            menuNum = 0;

            if (sf::IntRect(150, 200, 250, 50).contains(sf::Mouse::getPosition(window))) {
                menu1.setColor(sf::Color::Blue);
                menuNum = 1;
            }
            if (sf::IntRect(150, 300, 250, 50).contains(sf::Mouse::getPosition(window))) {
                menu2.setColor(sf::Color::Blue);
                menuNum = 3;
            }

//            window.popGLStates();
//            window.resetGLStates();
            window.draw(menuBg);
            window.draw(menu1);
            window.draw(menu2);

//            window.pushGLStates();

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (menuNum == 1) {
                    isMenuLive = false;
                }
                if (menuNum == 3) {
                    isMenuLive = false;
                }
            }
        } else {
            float currTime = gameClock.getElapsedTime().asSeconds();

            if (currTime - lastUpdateTime > constTimeStep) {
                lastUpdateTime += constTimeStep;
                elapsedTime += constTimeStep;
//                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//                gameSystem.Update(constTimeStep, buttonsCache);
            }
        }

        window.display();
    }

    window.close();

    return 0;
}

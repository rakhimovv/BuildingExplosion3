#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "WindowSize.h"
#include "GameObjects/GameSystem.h"
#include <queue>

#include "../dependencies/json/json.h"
#include "GameParameters.h"

void Menu(sf::RenderWindow &window) {
    bool isMenu = 1;
    int menuNum = 0;

    sf::Texture menuTexture1, menuTexture2, menuBackground;
    menuTexture1.loadFromFile("data/start.png");
    menuTexture2.loadFromFile("data/exit.png");
    menuBackground.loadFromFile("data/background.jpg");

    sf::Sprite menu1(menuTexture1), menu2(menuTexture2), menuBg(menuBackground);
    menu1.setPosition(150, 200);
    menu2.setPosition(150, 300);
    menuBg.setPosition(0, 0);


    while (isMenu & window.isOpen()) {
        menu1.setColor(sf::Color::White);
        menu2.setColor(sf::Color::White);
        menuNum = 0;
        window.clear(sf::Color(240, 222, 179));

        if (sf::IntRect(150, 200, 250, 50).contains(sf::Mouse::getPosition(window))) {
            menu1.setColor(sf::Color::Blue);
            menuNum = 1;
        }
        if (sf::IntRect(150, 300, 250, 50).contains(sf::Mouse::getPosition(window))) {
            menu2.setColor(sf::Color::Blue);
            menuNum = 3;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (menuNum == 1) isMenu = false;
            if (menuNum == 3) {
                //window.close();
                isMenu = false;
            }

        }
        sf::Event evt;
        while (window.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.draw(menuBg);
        window.draw(menu1);
        window.draw(menu2);

        window.display();
    }
}

int main()
{
    sf::RenderWindow enterWindow(sf::VideoMode(WIDTH, HEIGHT), "BuildingExplosion3");
    Menu(enterWindow);
    enterWindow.close();

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "BuildingExplosion3", sf::Style::Default,
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

    //Menu(window);

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

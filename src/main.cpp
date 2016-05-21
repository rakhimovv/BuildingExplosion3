#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "WindowSize.h"
#include "GameObjects/GameSystem.h"
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFGUI/SFGUI.hpp>
#include<SFGUI/Renderers.hpp>
#include<SFGUI/Widgets.hpp>
#include <queue>

#include "../dependencies/json/json.h"
#include "GameParameters.h"

GameSystem *menuSystem;

void OnCylinder() {
    std::cout << "Something1\n";
    menuSystem->OnCylinder();
}

void OnHyperboloid() {
    std::cout << "Something2\n";
    menuSystem->OnHyperboloid();
}

void OnNewBomb() {
    std::cout << "Something3\n";
    menuSystem->OnNewBomb();
}

void OnExit() {
    menuSystem->OnExit();
    //exit(0);
}

void OnBoom() {
    menuSystem->OnBoom();
    std::cout << "KABOOOOM\n";
}

sfg::Window::Ptr enableSFGUI(sf::RenderWindow &window, GameSystem *gameSystem) {
    window.clear();

    menuSystem = gameSystem;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) std::cout << "OK";

    window.resetGLStates();
    //window.pushGLStates();

    //sfg::SFGUI sfgui;

    sfg::Window::Ptr guiWindow = sfg::Window::Create();

    guiWindow->SetTitle("Menu?");

    auto table = sfg::Table::Create();
    table->SetRowSpacings(5.f);
    table->SetColumnSpacings(5.f);

    sfg::Button::Ptr map1 = sfg::Button::Create("Cylinder");
    sfg::Button::Ptr map2 = sfg::Button::Create("Hyperboloid");
    sfg::Button::Ptr map3 = sfg::Button::Create("New bomb");
    sfg::Button::Ptr boom = sfg::Button::Create("BOOM");
    sfg::Button::Ptr ext = sfg::Button::Create("exit");
    auto Scale = sfg::Scale::Create(0.f, 100.f, 1.f, sfg::Scale::Orientation::HORIZONTAL);
    sfg::CheckButton::Ptr check = sfg::CheckButton::Create("Check me");

    ext->SetId("BUTTON5");

    table->Attach(map1, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL,
                  sf::Vector2f(10.f, 10.f));
    table->Attach(map2, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL,
                  sf::Vector2f(10.f, 10.f));
    table->Attach(map3, sf::Rect<sf::Uint32>(0, 2, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL,
                  sf::Vector2f(10.f, 10.f));
    table->Attach(ext, sf::Rect<sf::Uint32>(0, 4, 2, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL,
                  sf::Vector2f(10.f, 10.f));
    table->Attach(Scale, sf::Rect<sf::Uint32>(0, 3, 2, 1), sfg::Table::FILL | sfg::Table::EXPAND,
                  sfg::Table::FILL | sfg::Table::EXPAND, sf::Vector2f(1.f, 1.f));
    table->Attach(boom, sf::Rect<sf::Uint32>(1, 0, 1, 3), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL,
                  sf::Vector2f(10.f, 10.f));

    map1->GetSignal(sfg::Button::OnLeftClick).Connect(OnCylinder);//gameSystem->OnCylinder());
    map2->GetSignal(sfg::Button::OnLeftClick).Connect(OnHyperboloid);//gameSystem->OnHyperboloid());
    map3->GetSignal(sfg::Button::OnLeftClick).Connect(OnNewBomb);//gameSystem->OnNewBomb());
    boom->GetSignal(sfg::Button::OnLeftClick).Connect(OnBoom);//gameSystem->OnExit());
    ext->GetSignal(sfg::Button::OnLeftClick).Connect(OnExit);//gameSystem->OnExit());

    guiWindow->Add(table);

    sfg::Desktop desktopx;
    guiWindow->SetId("DESKTOP");
    guiWindow->SetPosition(sf::Vector2f(200, 400));
    desktopx.SetProperty("Button#BUTTON5", "BackgroundColor", sf::Color(175, 0, 0, 200));
    desktopx.SetProperty("Button#BUTTON5", "BorderWidth", 1.f);
    desktopx.SetProperty("Window", "BackgroundColor", sf::Color(80, 80, 80, 100));
    desktopx.SetProperty("Window", "BorderColor", sf::Color(80, 80, 80, 200));
    desktopx.SetProperty("Window", "BorderWidth", 2.0f);
    desktopx.SetProperty("Window", "TitleBackgroundColor", sf::Color::Black);
    desktopx.Add(guiWindow);

    //window.popGLStates();

    return guiWindow;
}

int main()
{
    //window2.setActive(false);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "BuildingExplosion3", sf::Style::Default,
                            sf::ContextSettings(32, 8, 4, 3, 0));

    //window2.setActive(true);


    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, window.getSize().x, window.getSize().y);
    glClearColor(1.0f, 0.1f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);


    //GameShader cubeShader("data/shaders/cubevertex.frag", "data/shaders/cubefragment.frag");

    sf::Clock gameClock;
    float elapsedTime = 0.0f;
    float lastUpdateTime = 0.0f;
    float constTimeStep = 1.0f / 60.0f;
    GameSystem gameSystem(constTimeStep);

    sf::RenderWindow window2(sf::VideoMode(500, 300), "Awesome window");

//    GameParameters gameParameters("data/gameconfig.json");

    sfg::SFGUI sfgui;
    sfg::Window::Ptr guiWindow = enableSFGUI(window2, &gameSystem);
    auto Scale = sfg::Scale::Create(0.f, 100.f, 1.f, sfg::Scale::Orientation::HORIZONTAL);


    // Очередь для нажатых клавиш (т.к. у нас есть промежутки между апдейтами)
    std::queue<sf::Keyboard::Key> buttonsCache;

    bool isRunning = true;
    while (isRunning && window2.isOpen()) {
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

        sf::Event evt;
        while (window2.pollEvent(evt)) {
            guiWindow->HandleEvent(evt);
            if (evt.type == sf::Event::Closed) {
                window2.close();
            }
        }

        float number(Scale->GetValue());

        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window.clear();
        window2.clear();

        guiWindow->Update(1e-2f);
        sfgui.Display(window2);

        //window.display();
        window2.display();

        float currTime = gameClock.getElapsedTime().asSeconds();
        if (currTime - lastUpdateTime > constTimeStep) {
            lastUpdateTime += constTimeStep;
            elapsedTime += constTimeStep;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            window.popGLStates();
            gameSystem.Update(constTimeStep, buttonsCache);
            window.pushGLStates();

            window.display();
        }


        /*
        float currTime = gameClock.getElapsedTime().asSeconds();
        if (currTime - lastUpdateTime > constTimeStep) {
            lastUpdateTime += constTimeStep;
            elapsedTime += constTimeStep;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            gameSystem.Update(constTimeStep, buttonsCache);

            window.display();
        }
         */
    }

    window.close();
    window2.close();

    return 0;
}

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "");
    window.setVerticalSyncEnabled(true);

    //Определяю вид для камеры игрока
    sf::View camera({ 640.f, 360.f }, { 1280.f, 720.f });
    sf::View mapView({ 3200.f, 1800.f }, { 6400.f, 3600.f });
    
    std::vector<sf::Vector2f> buildPos = {}; //список со всеми разм зданиями


    //спрайты
    sf::Texture play("play.png");
    sf::Texture Menu("menu.png");
    sf::Texture exit("exit.png");
    sf::Texture panel("SolarPanel.png");
    sf::Texture worldmap("Map.png");

    sf::Sprite WorldMap(worldmap);
    sf::Sprite SolarPanel(panel);
    sf::Sprite playButton(play);
    sf::Sprite menuButton(Menu);
    sf::Sprite exitButton(exit);
    SolarPanel.setScale({ 0.5f, 0.5f });

    playButton.setPosition({ 300, 400 });
    menuButton.setPosition({ 1180, 0 });
    exitButton.setPosition({ 780, 400 });
    WorldMap.setPosition({ -2360, -1440 });
    
    enum windowStates {
        menu,
        game
    };
    enum windowStates state = menu;
    while (window.isOpen())
    {
        sf::Vector2i MousePosition = sf::Mouse::getPosition(window);
        float MousePositionX = MousePosition.x;
        float MousePositionY = MousePosition.y;
        while (const std::optional event = window.pollEvent())
        {
            //Когда открыто меню
            if (event->is<sf::Event::Closed>())
                window.close();
            if (MousePositionX > 300 && MousePositionX < 500 && MousePositionY > 400 && MousePositionY < 600 && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && state == menu) {
                state = game;
            }
            if (MousePositionX > 780 && MousePositionX < 980 && MousePositionY > 400 && MousePositionY < 600 && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && state == menu) {
                window.close();
            }
            
            // Когда игра запущена
            if (state == game){
                if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                        sf::Vector2f MapMousePosition = window.mapPixelToCoords(MousePosition, mapView);
                        int MapMousePositionX = MapMousePosition.x;
                        int MapMousePositionY = MapMousePosition.y;
                        int tileIndexX = MapMousePositionX / 200;
                        int tileIndexY = MapMousePositionY / 200;
                        if (tileIndexX > 6 && tileIndexX < 24) {
                            sf::Vector2f newPos(tileIndexX * 200, tileIndexY * 200);
                            buildPos.push_back(newPos);
                            
                            /*
                            for (int i = 0; i < buildPos.size(); i++) {
                                std::cout << buildPos[i].y;
                            }
                            std::cout << '\n'; //- код для дебаггинга
                            */
                        }
                    }
                }
                if (MousePositionX > 1180 && MousePositionX < 1280 && MousePositionY > 0 && MousePositionY < 100 && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    state = menu;
                }
                if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                    if (mouseWheel->delta > 0 && camera.getSize().x > 800) {
                        camera.zoom(0.95f);
                    }
                    else if (mouseWheel->delta < 0 && camera.getSize().x < 6000) {
                        camera.zoom(1.1f);
                    }
                }
                if (camera.getSize().x >= 6000) camera.setSize({ 6000, 3600 });
            }
        }
        if (state == menu) {
            window.setView(window.getDefaultView());
            window.clear(sf::Color(60, 15, 255, 255));
            window.draw(playButton);
            window.draw(exitButton);
            window.display();
        }
        else if (state == game) {
            window.clear();
            window.setView(camera);
            window.draw(WorldMap);
            for (int i = 0; i < buildPos.size(); i++) {
                SolarPanel.setPosition(buildPos[i]);
                window.draw(SolarPanel);
            }

            window.setView(window.getDefaultView());
            window.draw(menuButton);
            window.display();
        }
    }
}
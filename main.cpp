#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "");
    window.setVerticalSyncEnabled(true);

    //Определяю вид для камеры игрока
    sf::View camera({ 640.f, 360.f }, { 1280.f, 720.f });
    
    sf::Font font("OpenSans.ttf");
    sf::Text text(font);

    sf::Texture play("play.png");
    sf::Texture Menu("menu.png");
    sf::Texture exit("exit.png");
    sf::Texture panel("SolarPanel.png");
    sf::Sprite SolarPanel(panel);
    sf::Sprite playButton(play);
    sf::Sprite menuButton(Menu);
    sf::Sprite exitButton(exit);
    
    std::string placedBuilding = "";

    playButton.setPosition({ 300, 400 });
    menuButton.setPosition({ 1080, 0 });
    exitButton.setPosition({ 780, 400 });

    enum windowStates {
        menu,
        game
    };
    enum windowStates state = menu;
    while (window.isOpen())
    {
        float MousePositionX = sf::Mouse::getPosition(window).x;
        float MousePositionY = sf::Mouse::getPosition(window).y;
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (MousePositionX > 300 && MousePositionX < 500 && MousePositionY > 400 && MousePositionY < 600 && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && state == menu) {
                state = game;
            }
            if (MousePositionX > 1080 && MousePositionX < 1280 && MousePositionY > 0 && MousePositionY < 200 && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && state == game) {
                state = menu;
            }
            if (MousePositionX > 780 && MousePositionX < 980 && MousePositionY > 400 && MousePositionY < 600 && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && state == menu) {
                window.close();
            }
            if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (mouseWheel->delta > 0 && camera.getSize().x > 800) {
                    camera.zoom(0.95f);
                }
                else if (mouseWheel->delta < 0 && camera.getSize().x < 6000) {
                    camera.zoom(1.1f);
                }
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
            window.setView(window.getDefaultView());
            window.draw(menuButton);
            window.setView(camera);
            window.draw(SolarPanel);
            window.display();
        }
    }
}
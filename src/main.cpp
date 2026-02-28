#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui-sfml/imgui-SFML.h"

int main(int argc, char* argv[])
{   
    const int WIDTH = 1280;
    const int HEIGHT = 720;

    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML Works!");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    int intVar = 10;
    int min = 0;
    int max = 15;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);  // Feed ImGui
            if (event->is<sf::Event::Closed>()) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart()); // Starts frame

        ImGui::Begin("Debug");
        ImGui::SliderInt("SomeInt", &intVar, min, max);
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0; 
}
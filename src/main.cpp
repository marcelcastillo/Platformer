#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui-sfml/imgui-SFML.h"


int main(int argc, char* argv[])
{   
    const int WIDTH = 1280;
    const int HEIGHT = 720;
    const float PI = 3.14159265;

    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML Works!");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    // Seed rand once

    std::srand(time(0));

    // Initialize Circle
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition({300.f, 200.f});

    // Random angle
    float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * PI;
    float speed = 250.0f;

    sf::Vector2f velocity;
    sf::Vector2f direction(
        std::cos(angle),
        std::sin(angle)
    );

    sf::Clock deltaClock;

    int intVar = 10;
    int min = 0;
    int max = 15;

    const char* items[] = {"AAAA", "BBBB", "CCCC", "DDDD"};
    static int item_index = 0;

    // Main game loop: Run while window is open
    while (window.isOpen()) {
        // Check all the window's events during last frame
        while (auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);  // Feed ImGui
            if (event->is<sf::Event::Closed>()) 
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }
        }
        sf::Time dt = deltaClock.restart();
        ImGui::SFML::Update(window, dt);

        /* Move Shape */
        velocity = direction * speed;
        shape.move(velocity * dt.asSeconds());

        // Bounce Logic
        sf::Vector2f pos = shape.getPosition();
        float radius = shape.getRadius();
        float diameter = radius * 2.0f;

        // Left & Right Walls
        if (pos.x <= 0.0f)
        {
            pos.x = 0.0f;
            direction.x = -direction.x;
        }
        else if (pos.x + diameter >= WIDTH)
        {
            pos.x = WIDTH - diameter;
            direction.x = - direction.x;
        }

        // Top / bottom walls
        if (pos.y <= 0.0f)
        {
            pos.y = 0.0f;
            direction.y = -direction.y;
        }
        else if (pos.y + diameter >= HEIGHT)
        {
            pos.y = HEIGHT - diameter;
            direction.y = -direction.y;
        }

        ImGui::Begin("Debug");
        ImGui::SliderFloat("Speed", &speed, 0.0f, 1000.0f);
        if (ImGui::Button("Button Text"))
        {
            // This Happens
        }
        ImGui::Combo("Combo", &item_index, items, IM_ARRAYSIZE(items));
        ImGui::Text("Velocity: %.2f, %.2f", velocity.x, velocity.y);
        ImGui::End();



        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0; 
}
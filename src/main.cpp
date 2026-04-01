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
#include "entities.h"

int main(int argc, char* argv[])
{
    const int WIDTH = 1280;
    const int HEIGHT = 720;
    const float PI = 3.14159265f;

    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML Works!");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    std::srand(std::time(0));

    /* Using Entities */
    Entity ball;
    ball.add<CTransform>(Vec2(300.f, 200.f), Vec2(0.f, 0.f));
    auto& shape = ball.add<CShape>(100.f, sf::Color::Green);

    float angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * PI;
    float speed = 250.0f;
    Vec2 direction(std::cos(angle), std::sin(angle));

    sf::Clock deltaClock;

    const char* items[] = {"AAAA", "BBBB", "CCCC", "DDDD"};
    static int item_index = 0;

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }
        }

        sf::Time dt = deltaClock.restart();
        ImGui::SFML::Update(window, dt);

        auto& transform = ball.get<CTransform>();
        auto& shape = ball.get<CShape>().shape;

        transform.velocity = direction * speed;
        transform.pos = transform.pos + transform.velocity * dt.asSeconds();

        float radius = shape.getRadius();
        float diameter = radius * 2.0f;

        if (transform.pos.x <= 0.0f)
        {
            transform.pos.x = 0.0f;
            direction.x = -direction.x;
        }
        else if (transform.pos.x + diameter >= WIDTH)
        {
            transform.pos.x = WIDTH - diameter;
            direction.x = -direction.x;
        }

        if (transform.pos.y <= 0.0f)
        {
            transform.pos.y = 0.0f;
            direction.y = -direction.y;
        }
        else if (transform.pos.y + diameter >= HEIGHT)
        {
            transform.pos.y = HEIGHT - diameter;
            direction.y = -direction.y;
        }

        shape.setPosition({transform.pos.x, transform.pos.y});

        ImGui::Begin("Debug");
        ImGui::SliderFloat("Speed", &speed, 0.0f, 1000.0f);
        ImGui::Combo("Combo", &item_index, items, IM_ARRAYSIZE(items));
        ImGui::Text("Velocity: %.2f, %.2f", transform.velocity.x, transform.velocity.y);
        ImGui::Text("Position: %.2f, %.2f", transform.pos.x, transform.pos.y);
        ImGui::End();

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
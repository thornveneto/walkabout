#pragma once

#include <SFML/Graphics.hpp>
#include "UI_InputEvent.h"

UI_InputEvent create_ui_event_from_input(sf::RenderWindow& window) {
    //---STAGE: READING INPUT
    UI_InputEvent ui_input_event;

    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            ui_input_event.close_window_request = true;
        }
        if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                ui_input_event.left_key_pressed = true;
            }
            if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                ui_input_event.right_key_pressed = true;
            }
        }
        if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
                ui_input_event.left_key_released = true;
            }
            if (mouseButtonReleased->button == sf::Mouse::Button::Right) {
                ui_input_event.right_key_released = true;
            }
        }
    }

    // MOUSE
    sf::Vector2i mouse_local_position = sf::Mouse::getPosition(window); // window is a sf::Window
    ui_input_event.mouse_position = { mouse_local_position.x, mouse_local_position.y };

    return ui_input_event;
}
#pragma once
#include <iostream>
#include <string>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "clock.hpp"
#include "config.hpp"
#include "camera.hpp"
#include "pixelbuffer.hpp"

class Terminal {
    private:
        int display_mode; 
        std::string grayscale;
        float camera_speed;
        float mouse_sensitivity;
        sf::Vector2i mouse_position;
        sf::Vector2i last_mouse_position;
        bool invert_mouse;
    public:
        explicit Terminal(Config& config);
        void display(PixelBuffer *pb);
        bool handle_events(Camera *camera, float frametime);
};
void show_cursor(bool show);
void go_to(int x, int y);
void reset_coloring();
void set_foreground_color(int r, int g, int b);
void set_background_color(int r, int g, int b);

#include "../include/terminal.hpp"

Terminal::Terminal(Config& config) {
    display_mode = config.terminal_display_mode;
    grayscale = config.renderer_grayscale;
    camera_speed = config.camera_speed;
    mouse_sensitivity = config.camera_mouse_sensitivity;
    last_mouse_position = sf::Mouse::getPosition();
    invert_mouse = config.camera_invert_mouse;
}

void show_cursor(bool show) {
    if (show) {
        printf("\033[?25h");
    } else {
        printf("\033[?25l");
    }
}

void set_foreground_color(int r, int g, int b) {
    printf("\033[38;2;%u;%u;%um", r, g, b);
}
void set_background_color(int r, int g, int b) {
    printf("\033[48;2;%u;%u;%um", r, g, b);
}

void reset_coloring() {
    std::cout << "\033[0m\t\t";
}

void go_to(int x, int y) {
    std::cout << "\033[" << x << ";" << y << "H";
}

void Terminal::display(PixelBuffer *pixelbuffer) {
    // set cursor position to top left corner
    go_to(0,0);
    switch (display_mode) {
        /*
           display_mode 0 - grayscale ascii
           this display mode skips every second row of the frame because it cant make use of the half-block like 
           display mode 1. Every row is still being rendered so it doesn't gain any speed either
           */
        case 0:
            // go through pixel array
            for (int y=0; y<pixelbuffer->height; y++) {
                for (int x=0; x<pixelbuffer->width; x++) {
                    // convert from RGB to grayscale
                    float brightness = 0.299*pixelbuffer->pixels[y][x][0] +
                        0.587*pixelbuffer->pixels[y][x][1] +
                        0.114*pixelbuffer->pixels[y][x][2];
                    brightness /= 256.; // so that brightness ranges from 0 to 1
                                        // choose the corresponding brightness character from the grayscale array
                    std::cout << grayscale[(int)(brightness*grayscale.length())];
                }
                y += 1;
                std::cout << "\n";
            }
            break;

            /*
               display_mode 1 - colored ascii
               same as display_mode 0 but with color
               */
        case 1:
            // go through pixel array
            for (int y=0; y<pixelbuffer->height; y++) {
                for (int x=0; x<pixelbuffer->width; x++) {
                    if ((x == 0) || (pixelbuffer->pixels[y][x] != pixelbuffer->pixels[y][x-1])) { 
                        set_foreground_color(
                                pixelbuffer->pixels[y][x][0], 
                                pixelbuffer->pixels[y][x][1],
                                pixelbuffer->pixels[y][x][2]);
                    }
                    // convert from RGB to grayscale
                    float brightness = 0.299*pixelbuffer->pixels[y][x][0] +
                        0.587*pixelbuffer->pixels[y][x][1] +
                        0.114*pixelbuffer->pixels[y][x][2];
                    brightness /= 256.; // so that brightness ranges from 0 to 1
                                        // choose the corresponding brightness character from the grayscale array
                    std::cout << grayscale[(int)(brightness*grayscale.length())];
                }
                y += 1;
                std::cout << "\n";
            }
            break;

            /*
               display_mode 2 - colored ANSI
               this display mode displays 2 pixel rows on 1 character row by using the lower half block ( \u2584 )
               the number of calls to set the color can be reduces by checking if the pixel color has changed
               */
        case 2:
            for (int y=0; y<pixelbuffer->height; y++) {
                for (int x=0; x<pixelbuffer->width; x++) {
                    // if first pixel of row or if color of current pixel is not the same as color of last pixel
                    if ((x == 0) || (pixelbuffer->pixels[y][x] != pixelbuffer->pixels[y][x-1])) {  
                        set_background_color(
                                pixelbuffer->pixels[y][x][0], 
                                pixelbuffer->pixels[y][x][1], 
                                pixelbuffer->pixels[y][x][2]);
                    }
                    if ((x == 0) || (pixelbuffer->pixels[y+1][x] != pixelbuffer->pixels[y+1][x-1])) { 
                        set_foreground_color(
                                pixelbuffer->pixels[y+1][x][0], 
                                pixelbuffer->pixels[y+1][x][1],
                                pixelbuffer->pixels[y+1][x][2]);
                    }
                    std::cout << "\u2584"; // print lower half block
                }
                y += 1;
                std::cout << "\n";
            }
            break;
        default:
            system("clear");
            std::cout << "display_modes:\n0 - grayscale ascii\n1 - colored ascii\n2 - colored ANSI\n";
    };
    reset_coloring();
}

bool Terminal::handle_events(Camera *camera, float frametime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { // forward
        camera->view_point = camera->view_point + camera->view_direction*camera_speed*frametime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { // backward
        camera->view_point = camera->view_point - camera->view_direction*camera_speed*frametime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { // left
        camera->view_point = camera->view_point + camera->view_left*camera_speed*frametime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { // right
        camera->view_point = camera->view_point - camera->view_left*camera_speed*frametime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) { // up
        camera->view_point = camera->view_point + camera->view_up*camera_speed*frametime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { // down
        camera->view_point = camera->view_point - camera->view_up*camera_speed*frametime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) { // zoom (decrease FOV)
        camera->FOV -= camera_speed*15.0f*frametime;
        camera->FOV = std::min(110.0f, std::max(1.0f, camera->FOV));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) { // zoom (increase FOV)
        camera->FOV += camera_speed*15.0f*frametime;
        camera->FOV = std::min(110.0f, std::max(1.0f, camera->FOV));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { // change display mode
        if (display_mode == 2) {
            display_mode = 0;
        } else {
            display_mode++;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { // quit
        return true;
    }

    /* mouse */
    // center mouse if its closet to the screen borders
    if (mouse_position.x < 10 || mouse_position.x > 1910 || mouse_position.y < 10 || mouse_position.y > 1070) {
        sf::Mouse::setPosition(sf::Vector2i(960, 540));
        last_mouse_position = sf::Mouse::getPosition();
    } else {
        last_mouse_position = mouse_position;
    }
    mouse_position = sf::Mouse::getPosition();
    sf::Vector2i movement = mouse_position-last_mouse_position;
    if (invert_mouse) {
        camera->view_angle_z -= (float)(movement.x)*mouse_sensitivity*frametime;
        camera->view_angle_x += (float)(movement.y)*mouse_sensitivity*frametime;
    } else {
        camera->view_angle_z += (float)(movement.x)*mouse_sensitivity*frametime;
        camera->view_angle_x -= (float)(movement.y)*mouse_sensitivity*frametime;
    }
    camera->view_angle_x = std::min(1.55f, std::max(-1.55f, camera->view_angle_x));
    camera->view_angle_changed();

    return false;
}

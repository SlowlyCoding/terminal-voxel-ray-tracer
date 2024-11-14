#include <iomanip>
#include <iostream>
#include "clock.hpp"
#include "config.hpp"
#include "terminal.hpp"
#include "material.hpp"
#include "renderer.hpp"

int main() {
    std::cout << "\n[WASD] - move\n";
    std::cout << "[Mouse] - look around\n";
    std::cout << "[Q/E] - move up and down\n";
    std::cout << "[PageUp/PageDown] - change FOV\n";
    std::cout << "[Space] - change display mode\n";
    std::cout << "[Esc] - quit\n";

    /* parse config file */
    Config config("config.json");

    PixelBuffer pixelbuffer(config);
    Camera camera(config);

    /* create voxel object */
    std::cout << "\ncreating object...\n";
    Object object(config);
    object.change_material_at_palette_index(1, new_material_standard(RGB("orange")));
    object.change_material_at_palette_index(2, new_material_standard(RGB("green")));
    object.change_material_at_palette_index(3, new_material_standard(RGB(42,42,42)));
    object.change_material_at_palette_index(4, new_material_reflective(RGB(64,224,208), 0.2));
    object.change_material_at_palette_index(5, new_material_standard(RGB("random")));
    std::cout << "Voxel model taking " << std::fixed << std::setprecision(2) 
              << (object.voxels.size())/1e6 << "MB of space\n";

    Renderer renderer(config, &pixelbuffer, &camera, &object);
    Clock clock(config);
    Terminal terminal(config);
    show_cursor(false);

    /* setup done */
    std::cout << "\nPress Enter to start\n";
    std::cin.ignore();

    /* main loop */
    bool quit = false;
    while (!quit) {
        clock.start();
        renderer.threaded_render();
        clock.finished_render();
        terminal.display(&pixelbuffer);
        clock.finished_display();
        quit = terminal.handle_events(&camera, clock.frametime);
        clock.finished_frame();
        clock.display_performance();
    }
    show_cursor(true);
    return 0;
}

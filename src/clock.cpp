#include <thread>
#include <iomanip>
#include "clock.hpp"

Clock::Clock(Config& config) {
    fps_limit_enabled = config.terminal_fps_limit_enabled;
    fps_limit = config.terminal_fps_limit;
}

void Clock::start() {
    t_start = std::chrono::high_resolution_clock::now();
}

void Clock::finished_render() {
    t_render = std::chrono::high_resolution_clock::now();
    rendertime = std::chrono::duration_cast<std::chrono::microseconds>(t_render-t_start).count()/1000000.;
}
void Clock::finished_display() {
    t_display = std::chrono::high_resolution_clock::now();
    displaytime = std::chrono::duration_cast<std::chrono::microseconds>(t_display-t_render).count()/1000000.;
}
void Clock::finished_frame() {
    t_frame = std::chrono::high_resolution_clock::now();
    frametime = std::chrono::duration_cast<std::chrono::microseconds>(t_frame-t_start).count()/1000000.;
    // limiting fps
    if (fps_limit_enabled) {
        std::this_thread::sleep_for(std::chrono::microseconds((int)(((1./fps_limit)-frametime)*1000000)));
    }
    // calculate new frametime
    t_frame = std::chrono::high_resolution_clock::now();
    frametime = std::chrono::duration_cast<std::chrono::microseconds>(t_frame-t_start).count()/1000000.;

    if (frame_counter == 120) {
        fps_combined /= 2;
        frame_counter /= 2;
    }
    fps_combined += round(1.0/frametime);
    frame_counter++;
}
void Clock::display_performance() {
    std::cout << "\rFPS:" << std::right << std::setw(4) << (int)round(1/frametime)
              << "   avgFPS:" << std::right << std::setw(4) << (int)round((fps_combined/frame_counter))
              << std::fixed << std::setprecision(1)
              << "   Render:" << std::right << std::setw(5) << rendertime*1000
              << "   Display:" << std::right << std::setw(5) << displaytime*1000
              << "   Frame:" << std::right << std::setw(5) << frametime*1000 << std::endl;
}

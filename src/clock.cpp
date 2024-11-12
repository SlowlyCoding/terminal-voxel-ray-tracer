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
    rendertime_str = std::to_string(rendertime*1000);
    rendertime_str = rendertime_str.substr(0, rendertime_str.find(".")+precision+1);
    displaytime_str = std::to_string(displaytime*1000);
    displaytime_str = displaytime_str.substr(0, displaytime_str.find(".")+precision+1);
    frametime_str = std::to_string(frametime*1000);
    frametime_str = frametime_str.substr(0, frametime_str.find(".")+precision+1);

    fps_str = std::string("\rFPS: ").append(std::to_string((int)round((1/frametime)))).append(std::string("  "));
    avgfps_str = std::string("avgFPS: ").append(std::to_string((int)round((fps_combined/frame_counter)))).append(std::string("  "));
    rendertime_str = std::string("Render: ").append(rendertime_str).append(std::string("ms  "));
    displaytime_str = std::string("Display: ").append(displaytime_str).append(std::string("ms  "));
    frametime_str = std::string("Frame: ").append(frametime_str).append(std::string("ms  "));
    std::cout << fps_str << avgfps_str << rendertime_str << displaytime_str << frametime_str << std::endl;
}

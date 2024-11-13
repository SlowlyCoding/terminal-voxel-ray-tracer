#pragma once
#include <chrono>
#include "config.hpp"

class Clock {
    private:
        std::chrono::high_resolution_clock::time_point t_start, t_render, t_display, t_frame; 
        double rendertime, displaytime; // seconds
        bool fps_limit_enabled;
        int fps_limit;
        long fps_combined = 0;
        long frame_counter = 0;
    public:
        double frametime; // seconds
        Clock(Config& config);
        void start();
        void finished_render();
        void finished_display();
        void finished_frame();
        void display_performance();
};



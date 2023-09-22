#pragma once

#include <Windows.h>

namespace EngineSimulator {
    class EngineSimulatorMain {
    public:
        EngineSimulatorMain(HWND hwnd);
        void run();

    private:
        void initialize();
        void update();
        void render();
        void calculate_forces();
        void update_grid();

        HWND m_hwnd;
        // TODO: Add DirectX 12 device and resource variables
        // TODO: Add dataset variables
    };
}
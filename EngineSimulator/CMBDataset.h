#pragma once

#include <DirectXMath.h>

using namespace DirectX;

const int N = 10; // Number of cells in each dimension
const float h = 1.0f; // Spacing between cells
const float rho_init = 1.0f; // Initial density
const float T_init = 2.7f; // Initial temperature
const float gamma_init = 1.4f; // Initial adiabatic index
const float G = 6.67430e-11f; // Gravitational constant

class CMBDataset {
public:
    CMBDataset();
    void initialize();
    void calculate_forces();
    void update_grid();

    float rho[N * N * N];
    float T[N * N * N];
    float gamma[N * N * N];
    float Fg[N * N * N][3];
    float Fe[N * N * N][3];
    float Fw[N * N * N][3];
    float Fs[N * N * N][3];
    float Fn[N * N * N][3];
};
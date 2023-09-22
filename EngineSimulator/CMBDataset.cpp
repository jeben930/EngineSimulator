#include "CMBDataset.h"

CMBDataset::CMBDataset() {
    initialize();
}

void CMBDataset::initialize(float inflation, float dark_matter, float dark_energy) {
    // Initialize the dataset with values based on a simplified model
    for (int i = 0; i < N * N * N; i++) {
        // Set the temperature of each cell based on the cosmic microwave background radiation
        float T0 = 2.7255f;
        float deltaT = 0.001f * sin(i % N) * sin((i / N) % N) * sin(i / (N * N));
        T[i] = T0 + deltaT;

        // Set the density of each cell based on the distribution of matter and energy in the universe
        float r = sqrt(pow((i % N) - N / 2, 2) + pow(((i / N) % N) - N / 2, 2) + pow((i / (N * N)) - N / 2, 2));
        float density = dark_matter * exp(-r / 10.0f) + dark_energy * exp(r / 10.0f) + inflation;
        rho[i] = density;

        // Set the charge of each cell to random values
        // Set the charge of each cell to a random value between -1 and 1
        q[i] = (float)rand() / RAND_MAX * 2.0f - 1.0f;
    }

    // Cancel out the charges as much as possible
    for (int i = 0; i < N * N * N; i++) {
        float total_charge = 0.0f;

        // Calculate the total charge in the neighborhood of each cell
        for (int j = 0; j < N * N * N; j++) {
            if (i == j) {
                continue;
            }

            float dx = (i % N - j % N) * h;
            float dy = ((i / N) % N - (j / N) % N) * h;
            float dz = (i / (N * N) - j / (N * N)) * h;

            float r = sqrt(dx * dx + dy * dy + dz * dz);

            if (r < 2.0f * h) {
                total_charge += q[j];
            }
        }

        // Adjust the charge of each cell to cancel out the total charge in its neighborhood
        q[i] -= total_charge / 26.0f;
    }

    // Adjust the gravity based on the distribution of dark matter structures
    for (int i = 0; i < N * N * N; i++) {
        float total_mass = 0.0f;

        // Calculate the total mass in the neighborhood of each cell
        for (int j = 0; j < N * N * N; j++) {
            if (i == j) {
                continue;
            }

            float dx = (i % N - j % N) * h;
            float dy = ((i / N) % N - (j / N) % N) * h;
            float dz = (i / (N * N) - j / (N * N)) * h;

            float r = sqrt(dx * dx + dy * dy + dz * dz);

            if (r < 2.0f * h) {
                float mass = rho[j] * h * h * h;
                total_mass += mass;
            }
        }

    // Adjust the gravity of each cell based on the total mass in its neighborhood
    float g = 6.6743e-11f;
    float M = total_mass;
    float r = h;
    float F = g * M / (r * r);
    float a = F / rho[i];
    g[i] = a;
    }
}

void CMBDataset::calculate_forces() {
    // Calculate gravity forces
    calculate_gravity();

    // Calculate electromagnetic forces
    calculate_electromagnetism();

    // Calculate weak nuclear forces
    calculate_weak_nuclear();

    // Calculate strong nuclear forces
    calculate_strong_nuclear();
}

void CMBDataset::update_grid() {
    // Update the positions and velocities of each particle based on the forces
    for (int i = 0; i < N * N * N; i++) {
        // Calculate the acceleration of each particle based on the forces acting on it
        float ax = Fx_grav[i] / m[i] + Fx_em[i] / m[i] + Fx_weak[i] / m[i] + Fx_strong[i] / m[i];
        float ay = Fy_grav[i] / m[i] + Fy_em[i] / m[i] + Fy_weak[i] / m[i] + Fy_strong[i] / m[i];
        float az = Fz_grav[i] / m[i] + Fz_em[i] / m[i] + Fz_weak[i] / m[i] + Fz_strong[i] / m[i];

        // Update the velocity of each particle based on the acceleration
        vx[i] += ax * dt;
        vy[i] += ay * dt;
        vz[i] += az * dt;

        // Update the position of each particle based on the velocity
        x[i] += vx[i] * dt;
        y[i] += vy[i] * dt;
        z[i] += vz[i] * dt;
    }
}
private:
    // Calculate gravity forces
    void CMBDataset::calculate_gravity() {
        // Calculate the forces on each cell in the dataset due to gravity
        for (int i = 0; i < N * N * N; i++) {
            Fg[i][0] = 0.0f;
            Fg[i][1] = 0.0f;
            Fg[i][2] = 0.0f;

            for (int j = 0; j < N * N * N; j++) {
                if (i == j) {
                    continue;
                }

                float dx = (i % N - j % N) * h;
                float dy = ((i / N) % N - (j / N) % N) * h;
                float dz = (i / (N * N) - j / (N * N)) * h;

                float r = sqrt(dx * dx + dy * dy + dz * dz);

                // Calculate the force on each cell in the dataset due to gravity
                float G = 6.67430e-11f; // Gravitational constant
                float M = 4.0f / 3.0f * M_PI * pow(R, 3) * rho0; // Total mass of the sphere
                float a = 1.0f + H * r - 0.5f * H * H * r * r; // Acceleration rate/inflation
                float Fg_ij = G * M * rho[j] / r / r * a;

                Fg[i][0] += Fg_ij * dx / r;
                Fg[i][1] += Fg_ij * dy / r;
                Fg[i][2] += Fg_ij * dz / r;
            }
        }
    }

    // Calculate electromagnetic forces
    void CMBDataset::calculate_electromagnetism() {
        float Fe_ij = k * q[i] * q[j] / r / r;

        Fe[i][0] += Fe_ij * dx / r;
        Fe[i][1] += Fe_ij * dy / r;
        Fe[i][2] += Fe_ij * dz / r;
    }

    // Calculate weak nuclear forces
    void CMBDataset::calculate_weak_nuclear() {

        // Calculate the force on each cell in the dataset due to weak nuclear force
        float Fw_ij = 0.0f;

        // Calculate the exchange of W and Z bosons
        float Wp = 1.0f / sqrt(2.0f);
        float Wm = -1.0f / sqrt(2.0f);
        float Z = 1.0f / sqrt(2.0f);

        float gL = 0.5f;
        float gR = 0.0f;

        float sin_theta_W = 0.231f;
        float cos_theta_W = sqrt(1.0f - sin_theta_W * sin_theta_W);

        float Vud = 0.974f;
        float Vus = 0.225f;
        float Vub = 0.004f;
        float Vcd = 0.225f;
        float Vcs = 0.973f;
        float Vcb = 0.041f;
        float Vtd = 0.009f;
        float Vts = 0.040f;
        float Vtb = 0.999f;

        float u = 2.0f / 3.0f;
        float d = -1.0f / 3.0f;
        float s = -1.0f / 3.0f;
        float c = 2.0f / 3.0f;
        float b = -1.0f / 3.0f;
        float t = 2.0f / 3.0f;

        float N = 3.0f * rho[i] / (4.0f * M_PI * pow(h, 3.0f));

        float E = 3.0f / 2.0f * T[i];

        float Y_e = 0.5f;

        float gA = 1.27f;

        float Fw_e = 0.0f;
        float Fw_u = 0.0f;
        float Fw_d = 0.0f;

        // Calculate the force on each cell in the dataset due to weak nuclear force
        if (rho[i] > 0.0f && rho[j] > 0.0f) {
            float Fw_eu = 0.0f;
            float Fw_ed = 0.0f;
            float Fw_ue = 0.0f;
            float Fw_de = 0.0f;

            float Fw_uu = 0.0f;
            float Fw_dd = 0.0f;

            float Fw_ee = 0.0f;

            float Fw_eu_L = 0.0f;
            float Fw_ed_L = 0.0f;
            float Fw_ue_L = 0.0f;
            float Fw_de_L = 0.0f;

            float Fw_uu_L = 0.0f;
            float Fw_dd_L = 0.0f;

            float Fw_ee_L = 0.0f;

            float Fw_eu_R = 0.0f;
            float Fw_ed_R = 0.0f;
            float Fw_ue_R = 0.0f;
            float Fw_de_R = 0.0f;

            float Fw_uu_R = 0.0f;
            float Fw_dd_R = 0.0f;

            float Fw_ee_R = 0.0f;

            float Fw_eu_V = 0.0f;
            float Fw_ed_V = 0.0f;
            float Fw_ue_V = 0.0f;
            float Fw_de_V = 0.0f;

            float Fw_uu_V = 0.0f;
            float Fw_dd_V = 0.0f;

            float Fw_ee_V = 0.0f;

            float Fw_eu_A = 0.0f;
            float Fw_ed_A = 0.0f;
            float Fw_ue_A = 0.0f;
            float Fw_de_A = 0.0f;

            float Fw_uu_A = 0.0f;
            float Fw_dd_A = 0.0f;

            float Fw_ee_A = 0.0f;

            float Fw_eu_S = 0.0f;
            float Fw_ed_S = 0.0f;
            float Fw_ue_S = 0.0f;
            float Fw_de_S = 0.0f;

            float Fw_uu_S = 0.0f;
            float Fw_dd_S = 0.0f;

            float Fw_ee_S = 0.0f;

            float Fw_eu_T = 0.0f;
            float Fw_ed_T = 0.0f;
            float Fw_ue_T = 0.0f;
            float Fw_de_T = 0.0f;

            float Fw_uu_T = 0.0f;
            float Fw_dd_T = 0.0f;

            float Fw_ee_T = 0.0f;
            // Calculate the exchange of W and Z bosons
            float M_W = 80.379f;
            float M_Z = 91.1876f;
            float G_F = 1.1663787e-5f;
            float sin2_theta_W = 0.231f;
            float cos2_theta_W = 1.0f - sin2_theta_W;
            float sin_theta_W = sqrt(sin2_theta_W);
            float cos_theta_W = sqrt(cos2_theta_W);

            // Calculate the force on each cell in the dataset due to weak nuclear force
            float Q_i = (1.0f - 4.0f * sin2_theta_W) * (2.0f * T3[i] - 4.0f * Q[i] * sin2_theta_W);
            float Q_j = (1.0f - 4.0f * sin2_theta_W) * (2.0f * T3[j] - 4.0f * Q[j] * sin2_theta_W);

            float V_ud = Vud * Vud;
            float V_us = Vus * Vus;
            float V_ub = Vub * Vub;
            float V_cd = Vcd * Vcd;
            float V_cs = Vcs * Vcs;
            float V_cb = Vcb * Vcb;
            float V_td = Vtd * Vtd;
            float V_ts = Vts * Vts;
            float V_tb = Vtb * Vtb;

            float Fw_eu_L = -gL * (V_ud * Fw_dd_L + V_us * Fw_ds_L + V_ub * Fw_db_L + V_cd * Fw_ud_L + V_cs * Fw_us_L + V_cb * Fw_ub_L + V_td * Fw_td_L + V_ts * Fw_ts_L + V_tb * Fw_tb_L);
            float Fw_ed_L = -gL * (V_ud * Fw_ud_L + V_us * Fw_us_L + V_ub * Fw_ub_L + V_cd * Fw_dd_L + V_cs * Fw_ds_L + V_cb * Fw_db_L + V_td * Fw_td_L + V_ts * Fw_ts_L + V_tb * Fw_tb_L);
            float Fw_ue_L = -gL * (V_ud * Fw_uu_L + V_us * Fw_us_L + V_ub * Fw_ub_L + V_cd * Fw_cu_L + V_cs * Fw_cs_L + V_cb * Fw_cb_L + V_td * Fw_tu_L + V_ts * Fw_ts_L + V_tb * Fw_tb_L);
            float Fw_de_L = -gL * (V_ud * Fw_cu_L + V_us * Fw_cs_L + V_ub * Fw_cb_L + V_cd * Fw_dd_L + V_cs * Fw_ds_L + V_cb * Fw_db_L + V_td * Fw_td_L + V_ts * Fw_ts_L + V_tb * Fw_tb_L);

            float Fw_uu_L = -gL * (V_ud * Fw_ud_L + V_us * Fw_us_L + V_ub * Fw_ub_L + V_cd * Fw_uu_L + V_cs * Fw_cu_L + V_cb * Fw_tb_L + V_td * Fw_tu_L + V_ts * Fw_ts_L + V_tb * Fw_tb_L);
            float Fw_dd_L = -gL * (V_ud * Fw_dd_L + V_us * Fw_ds_L + V_ub * Fw_db_L + V_cd * Fw_dd_L + V_cs * Fw_cd_L + V_cb * Fw_sb_L + V_td * Fw_td_L + V_ts * Fw_ts_L + V_tb * Fw_tb_L);

            float Fw_ee_L = -gL * (Fw_eu_L + Fw_ed_L);

            float Fw_eu_R = -gR * (V_ud * Fw_dd_R + V_us * Fw_ds_R + V_ub * Fw_db_R + V_cd * Fw_ud_R + V_cs * Fw_us_R + V_cb * Fw_ub_R + V_td * Fw_td_R + V_ts * Fw_ts_R + V_tb * Fw_tb_R);
            float Fw_ed_R = -gR * (V_ud * Fw_ud_R + V_us * Fw_us_R + V_ub * Fw_ub_R + V_cd * Fw_dd_R + V_cs * Fw_ds_R + V_cb * Fw_db_R + V_td * Fw_td_R + V_ts * Fw_ts_R + V_tb * Fw_tb_R);
            float Fw_ue_R = -gR * (V_ud * Fw_uu_R + V_us * Fw_us_R + V_ub * Fw_ub_R + V_cd * Fw_cu_R + V_cs * Fw_cs_R + V_cb * Fw_cb_R + V_td * Fw_tu_R + V_ts * Fw_ts_R + V_tb * Fw_tb_R);
            float Fw_de_R = -gR * (V_ud * Fw_cu_R + V_us * Fw_cs_R + V_ub * Fw_cb_R + V_cd * Fw_dd_R + V_cs * Fw_ds_R + V_cb * Fw_db_R + V_td * Fw_td_R + V_ts * Fw_ts_R + V_tb * Fw_tb_R);

            float Fw_uu_R = -gR * (V_ud * Fw_ud_R + V_us * Fw_us_R + V_ub * Fw_ub_R + V_cd * Fw_uu_R + V_cs * Fw_cu_R + V_cb * Fw_tb_R + V_td * Fw_tu_R + V_ts * Fw_ts_R + V_tb * Fw_tb_R);
            float Fw_dd_R = -gR * (V_ud * Fw_dd_R + V_us * Fw_ds_R + V_ub * Fw_db_R + V_cd * Fw_dd_R + V_cs * Fw_cd_R + V_cb * Fw_sb_R + V_td * Fw_td_R + V_ts * Fw_ts_R + V_tb * Fw_tb_R);

            float Fw_ee_R = -gR * (Fw_eu_R + Fw_ed_R);

            float Fw_eu_V = -2.0f * gL * gR * (V_ud * Fw_dd_V + V_us * Fw_ds_V + V_ub * Fw_db_V + V_cd * Fw_ud_V + V_cs * Fw_us_V + V_cb * Fw_ub_V + V_td * Fw_td_V + V_ts * Fw_ts_V + V_tb * Fw_tb_V);
            float Fw_ed_V = -2.0f * gL * gR * (V_ud * Fw_ud_V + V_us * Fw_us_V + V_ub * Fw_ub_V + V_cd * Fw_dd_V + V_cs * Fw_ds_V + V_cb * Fw_db_V + V_td * Fw_td_V + V_ts * Fw_ts_V + V_tb * Fw_tb_V);
            float Fw_ue_V = -2.0f * gL * gR * (V_ud * Fw_uu_V + V_us * Fw_us_V + V_ub * Fw_ub_V + V_cd * Fw_cu_V + V_cs * Fw_cs_V + V_cb * Fw_cb_V + V_td * Fw_tu_V + V_ts * Fw_ts_V + V_tb * Fw_tb_V);
            float Fw_de_V = -2.0f * gL * gR * (V_ud * Fw_cu_V + V_us * Fw_cs_V + V_ub * Fw_cb_V + V_cd * Fw_dd_V + V_cs * Fw_ds_V + V_cb * Fw_db_V + V_td * Fw_td_V + V_ts * Fw_ts_V + V_tb * Fw_tb_V);

            float Fw_uu_V = -2.0f * gL * gR * (V_ud * Fw_ud_V + V_us * Fw_us_V + V_ub * Fw_ub_V + V_cd * Fw_uu_V + V_cs * Fw_cu_V + V_cb * Fw_tb_V + V_td * Fw_tu_V + V_ts * Fw_ts_V + V_tb * Fw_tb_V);
            float Fw_dd_V = -2.0f * gL * gR * (V_ud * Fw_dd_V + V_us * Fw_ds_V + V_ub * Fw_db_V + V_cd * Fw_dd_V + V_cs * Fw_cd_V + V_cb * Fw_sb_V + V_td * Fw_td_V + V_ts * Fw_ts_V + V_tb * Fw_tb_V);

            float Fw_ee_V = -2.0f * gL * gR * (Fw_eu_V + Fw_ed_V);

            float Fw_eu = Fw_eu_L + Fw_eu_R + Fw_eu_V;
            float Fw_ed = Fw_ed_L + Fw_ed_R + Fw_ed_V;
            float Fw_ue = Fw_ue_L + Fw_ue_R + Fw_ue_V;
            float Fw_de = Fw_de_L + Fw_de_R + Fw_de_V;
            float Fw_uu = Fw_uu_L + Fw_uu_R + Fw_uu_V;
            float Fw_dd = Fw_dd_L + Fw_dd_R + Fw_dd_V;
            float Fw_ee = Fw_ee_L + Fw_ee_R + Fw_ee_V;

            // Calculate the force on each cell in the dataset due to weak nuclear force and strong nuclear force
            Fw[i][0] += Fw_eu + Fw_ue;
            Fw[j][0] += Fw_ed + Fw_de;

            Fw[i][1] += Fw_eu + Fw_ue;
            Fw[j][1] += Fw_ed + Fw_de;

            Fw[i][2] += Fw_eu + Fw_ue;
            Fw[j][2] += Fw_ed + Fw_de;

            // Calculate the force on each cell in the dataset due to the total nuclear force
            Fn[i][0] = Fg[i][0] + Fe[i][0] + Fw[i][0] + Fs[i][0];
            Fn[j][0] = Fg[j][0] + Fe[j][0] + Fw[j][0] + Fs[j][0];

            Fn[i][1] = Fg[i][1] + Fe[i][1] + Fw[i][1] + Fs[i][1];
            Fn[j][1] = Fg[j][1] + Fe[j][1] + Fw[j][1] + Fs[j][1];

            Fn[i][2] = Fg[i][2] + Fe[i][2] + Fw[i][2] + Fs[i][2];
            Fn[j][2] = Fg[j][2] + Fe[j][2] + Fw[j][2] + Fs[j][2];
        }
    }

// Calculate strong nuclear forces
    void CMBDataset::CMBDataset::calculate_strong_nuclear() {
        // Calculate the forces on each cell in the dataset due to the strong nuclear force
        for (int i = 0; i < N * N * N; i++) {
            Fs[i][0] = 0.0f;
            Fs[i][1] = 0.0f;
            Fs[i][2] = 0.0f;

            for (int j = 0; j < N * N * N; j++) {
                if (i == j) {
                    continue;
                }

                float dx = (i % N - j % N) * h;
                float dy = ((i / N) % N - (j / N) % N) * h;
                float dz = (i / (N * N) - j / (N * N)) * h;

                float r = sqrt(dx * dx + dy * dy + dz * dz);

                // Calculate the force on each cell in the dataset due to the exchange of gluons
                float alpha_s = 0.118f; // Strong coupling constant
                float Fg_ij = alpha_s * q[i] * q[j] / r / r;

                Fs[i][0] += Fg_ij * dx / r;
                Fs[i][1] += Fg_ij * dy / r;
                Fs[i][2] += Fg_ij * dz / r;
            }
        }
    }
    }
;

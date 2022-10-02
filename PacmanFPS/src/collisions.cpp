#include "collisions.h"

// teste de colisão com o labirinto
int CollisionMaze(glm::vec4 pacman_position)
{
    // criar um "cubo" de tamanho 2 ao redor do pacman
    // definindo os 8 vertices do cubo
    glm::vec4 pacman_points[8] = {pacman_position + glm::vec4( 1.0, 1.0, 1.0,0.0f),
                                  pacman_position + glm::vec4( 1.0, 1.0,-1.0,0.0f),
                                  pacman_position + glm::vec4( 1.0,-1.0, 1.0,0.0f),
                                  pacman_position + glm::vec4( 1.0,-1.0,-1.0,0.0f),
                                  pacman_position + glm::vec4(-1.0, 1.0, 1.0,0.0f),
                                  pacman_position + glm::vec4(-1.0, 1.0,-1.0,0.0f),
                                  pacman_position + glm::vec4(-1.0,-1.0, 1.0,0.0f),
                                  pacman_position + glm::vec4(-1.0,-1.0,-1.0,0.0f)};

    // nomes definidos conforme imagem em nomes_labirinto.png
    // colisão com R1
    glm::vec4 R1_a = glm::vec4(1.0f, 0.0f, -6.0f, 1.0f);
    glm::vec4 R1_b = glm::vec4(7.5f, 3.5f,  6.0f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= R1_a.x && pacman_points[i].x <= R1_b.x &&
            pacman_points[i].y >= R1_a.y && pacman_points[i].y <= R1_b.y &&
            pacman_points[i].z >= R1_a.z && pacman_points[i].z <= R1_b.z) {
            return 1;
        }
    }

    // colisão com R2
    glm::vec4 R2_a = glm::vec4(-4.75f, 0.0f,-11.75f, 1.0f);
    glm::vec4 R2_b = glm::vec4( 2.25f, 3.5f,-10.50f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= R2_a.x && pacman_points[i].x <= R2_b.x &&
            pacman_points[i].y >= R2_a.y && pacman_points[i].y <= R2_b.y &&
            pacman_points[i].z >= R2_a.z && pacman_points[i].z <= R2_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= R2_a.x && pacman_points[7].x <= R2_b.x &&
        pacman_points[7].z < R2_a.z && pacman_points[0].z > R2_b.z) {
        return 1;
    }

    // colisão com R3
    glm::vec4 R3_a = glm::vec4(-4.75f, 0.0f, 10.50f, 1.0f);
    glm::vec4 R3_b = glm::vec4( 2.25f, 3.5f, 11.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= R3_a.x && pacman_points[i].x <= R3_b.x &&
            pacman_points[i].y >= R3_a.y && pacman_points[i].y <= R3_b.y &&
            pacman_points[i].z >= R3_a.z && pacman_points[i].z <= R3_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= R3_a.x && pacman_points[7].x <= R3_b.x &&
        pacman_points[7].z < R3_a.z && pacman_points[0].z > R3_b.z) {
        return 1;
    }

    // colisão com R4
    glm::vec4 R4_a = glm::vec4(-10.25f, 0.0f,-11.75f, 1.0f);
    glm::vec4 R4_b = glm::vec4( -9.00f, 3.5f, -4.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= R4_a.x && pacman_points[i].x <= R4_b.x &&
            pacman_points[i].y >= R4_a.y && pacman_points[i].y <= R4_b.y &&
            pacman_points[i].z >= R4_a.z && pacman_points[i].z <= R4_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= R4_a.z && pacman_points[7].z <= R4_b.z &&
        pacman_points[7].x < R4_a.x && pacman_points[0].x > R4_b.x) {
        return 1;
    }

    // colisão com R5
    glm::vec4 R5_a = glm::vec4(-10.25f, 0.0f, 4.75f, 1.0f);
    glm::vec4 R5_b = glm::vec4( -9.00f, 3.5f,11.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= R5_a.x && pacman_points[i].x <= R5_b.x &&
            pacman_points[i].y >= R5_a.y && pacman_points[i].y <= R5_b.y &&
            pacman_points[i].z >= R5_a.z && pacman_points[i].z <= R5_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= R5_a.z && pacman_points[7].z <= R5_b.z &&
        pacman_points[7].x < R5_a.x && pacman_points[0].x > R5_b.x) {
        return 1;
    }

    // colisão com R6
    glm::vec4 R6_a = glm::vec4(23.25f, 0.0f,-20.75f, 1.0f);
    glm::vec4 R6_b = glm::vec4(25.75f, 3.5f,-16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= R6_a.x && pacman_points[i].x <= R6_b.x &&
            pacman_points[i].y >= R6_a.y && pacman_points[i].y <= R6_b.y &&
            pacman_points[i].z >= R6_a.z && pacman_points[i].z <= R6_b.z) {
            return 1;
        }
    }

    // colisão com R7
    glm::vec4 R7_a = glm::vec4(23.25f, 0.0f,-11.75f, 1.0f);
    glm::vec4 R7_b = glm::vec4(25.75f, 3.5f, -4.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= R7_a.x && pacman_points[i].x <= R7_b.x &&
            pacman_points[i].y >= R7_a.y && pacman_points[i].y <= R7_b.y &&
            pacman_points[i].z >= R7_a.z && pacman_points[i].z <= R7_b.z) {
            return 1;
        }
    }

    // colisão com R8
    glm::vec4 R8_a = glm::vec4(17.50f, 0.0f,-20.75f, 1.0f);
    glm::vec4 R8_b = glm::vec4(18.75f, 3.5f,-16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= R8_a.x && pacman_points[i].x <= R8_b.x &&
            pacman_points[i].y >= R8_a.y && pacman_points[i].y <= R8_b.y &&
            pacman_points[i].z >= R8_a.z && pacman_points[i].z <= R8_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= R8_a.z && pacman_points[7].z <= R8_b.z &&
        pacman_points[7].x < R8_a.x && pacman_points[0].x > R8_b.x) {
        return 1;
    }

    // colisão com R9
    glm::vec4 R9_a = glm::vec4(23.25f, 0.0f,  4.75f, 1.0f);
    glm::vec4 R9_b = glm::vec4(25.75f, 3.5f, 11.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= R9_a.x && pacman_points[i].x <= R9_b.x &&
            pacman_points[i].y >= R9_a.y && pacman_points[i].y <= R9_b.y &&
            pacman_points[i].z >= R9_a.z && pacman_points[i].z <= R9_b.z) {
            return 1;
        }
    }

    // colisão com R10
    glm::vec4 R10_a = glm::vec4(23.25f, 0.0f, 16.00f, 1.0f);
    glm::vec4 R10_b = glm::vec4(25.75f, 3.5f, 20.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= R10_a.x && pacman_points[i].x <= R10_b.x &&
            pacman_points[i].y >= R10_a.y && pacman_points[i].y <= R10_b.y &&
            pacman_points[i].z >= R10_a.z && pacman_points[i].z <= R10_b.z) {
            return 1;
        }
    }

    // colisão com R11
    glm::vec4 R11_a = glm::vec4(17.50f, 0.0f, 16.00f, 1.0f);
    glm::vec4 R11_b = glm::vec4(18.75f, 3.5f, 20.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= R11_a.x && pacman_points[i].x <= R11_b.x &&
            pacman_points[i].y >= R11_a.y && pacman_points[i].y <= R11_b.y &&
            pacman_points[i].z >= R11_a.z && pacman_points[i].z <= R11_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= R11_a.z && pacman_points[7].z <= R11_b.z &&
        pacman_points[7].x < R11_a.x && pacman_points[0].x > R11_b.x) {
        return 1;
    }

    // colisão com T1
    glm::vec4 T1a_a = glm::vec4(-4.75f, 0.0f, -6.0f, 1.0f);
    glm::vec4 T1a_b = glm::vec4(-3.50f, 3.5f,  6.0f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T1a_a.x && pacman_points[i].x <= T1a_b.x &&
            pacman_points[i].y >= T1a_a.y && pacman_points[i].y <= T1a_b.y &&
            pacman_points[i].z >= T1a_a.z && pacman_points[i].z <= T1a_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= T1a_a.z && pacman_points[7].z <= T1a_b.z &&
        pacman_points[7].x < T1a_a.x && pacman_points[0].x > T1a_b.x) {
        return 1;
    }

    glm::vec4 T1b_a = glm::vec4(-10.25f, 0.0f, -0.75f, 1.0f);
    glm::vec4 T1b_b = glm::vec4(-3.50f, 3.5f, 0.5f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T1b_a.x && pacman_points[i].x <= T1b_b.x &&
            pacman_points[i].y >= T1b_a.y && pacman_points[i].y <= T1b_b.y &&
            pacman_points[i].z >= T1b_a.z && pacman_points[i].z <= T1b_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= T1b_a.x && pacman_points[7].x <= T1b_b.x &&
        pacman_points[7].z < T1b_a.z && pacman_points[0].z > T1b_b.z) {
        return 1;
    }

    // colisão com T2
    glm::vec4 T2a_a = glm::vec4(-15.75f, 0.0f, -6.0f, 1.0f);
    glm::vec4 T2a_b = glm::vec4(-14.50f, 3.5f,  6.0f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T2a_a.x && pacman_points[i].x <= T2a_b.x &&
            pacman_points[i].y >= T2a_a.y && pacman_points[i].y <= T2a_b.y &&
            pacman_points[i].z >= T2a_a.z && pacman_points[i].z <= T2a_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= T2a_a.z && pacman_points[7].z <= T2a_b.z &&
        pacman_points[7].x < T2a_a.x && pacman_points[0].x > T2a_b.x) {
        return 1;
    }

    glm::vec4 T2b_a = glm::vec4(-21.50f, 0.0f, -0.75f, 1.0f);
    glm::vec4 T2b_b = glm::vec4(-14.50f, 3.5f, 0.5f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T2b_a.x && pacman_points[i].x <= T2b_b.x &&
            pacman_points[i].y >= T2b_a.y && pacman_points[i].y <= T2b_b.y &&
            pacman_points[i].z >= T2b_a.z && pacman_points[i].z <= T2b_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= T2b_a.x && pacman_points[7].x <= T2b_b.x &&
        pacman_points[7].z < T2b_a.z && pacman_points[0].z > T2b_b.z) {
        return 1;
    }

    // colisão com T3
    glm::vec4 T3a_a = glm::vec4(-21.50f, 0.0f, -20.75f, 1.0f);
    glm::vec4 T3a_b = glm::vec4(-20.25f, 3.5f, -4.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T3a_a.x && pacman_points[i].x <= T3a_b.x &&
            pacman_points[i].y >= T3a_a.y && pacman_points[i].y <= T3a_b.y &&
            pacman_points[i].z >= T3a_a.z && pacman_points[i].z <= T3a_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= T3a_a.z && pacman_points[7].z <= T3a_b.z &&
        pacman_points[7].x < T3a_a.x && pacman_points[0].x > T3a_b.x) {
        return 1;
    }

    glm::vec4 T3b_a = glm::vec4(-21.50f, 0.0f, -11.75f, 1.0f);
    glm::vec4 T3b_b = glm::vec4(-14.50f, 3.5f, -10.50f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T3b_a.x && pacman_points[i].x <= T3b_b.x &&
            pacman_points[i].y >= T3b_a.y && pacman_points[i].y <= T3b_b.y &&
            pacman_points[i].z >= T3b_a.z && pacman_points[i].z <= T3b_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= T3b_a.x && pacman_points[7].x <= T3b_b.x &&
        pacman_points[7].z < T3b_a.z && pacman_points[0].z > T3b_b.z) {
        return 1;
    }

    // colisão com T4
    glm::vec4 T4a_a = glm::vec4(-21.50f, 0.0f, 4.75f, 1.0f);
    glm::vec4 T4a_b = glm::vec4(-20.25f, 3.5f, 20.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T4a_a.x && pacman_points[i].x <= T4a_b.x &&
            pacman_points[i].y >= T4a_a.y && pacman_points[i].y <= T4a_b.y &&
            pacman_points[i].z >= T4a_a.z && pacman_points[i].z <= T4a_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= T4a_a.z && pacman_points[7].z <= T4a_b.z &&
        pacman_points[7].x < T4a_a.x && pacman_points[0].x > T4a_b.x) {
        return 1;
    }

    glm::vec4 T4b_a = glm::vec4(-21.50f, 0.0f, 10.50f, 1.0f);
    glm::vec4 T4b_b = glm::vec4(-14.50f, 3.5f, 11.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T4b_a.x && pacman_points[i].x <= T4b_b.x &&
            pacman_points[i].y >= T4b_a.y && pacman_points[i].y <= T4b_b.y &&
            pacman_points[i].z >= T4b_a.z && pacman_points[i].z <= T4b_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= T4b_a.x && pacman_points[7].x <= T4b_b.x &&
        pacman_points[7].z < T4b_a.z && pacman_points[0].z > T4b_b.z) {
        return 1;
    }

    // colisão com T5
    glm::vec4 T5a_a = glm::vec4(11.50f, 0.0f, -11.75f, 1.0f);
    glm::vec4 T5a_b = glm::vec4(12.75f, 3.5f, -4.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T5a_a.x && pacman_points[i].x <= T5a_b.x &&
            pacman_points[i].y >= T5a_a.y && pacman_points[i].y <= T5a_b.y &&
            pacman_points[i].z >= T5a_a.z && pacman_points[i].z <= T5a_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= T5a_a.z && pacman_points[7].z <= T5a_b.z &&
        pacman_points[7].x < T5a_a.x && pacman_points[0].x > T5a_b.x) {
        return 1;
    }

    glm::vec4 T5b_a = glm::vec4(6.50f, 0.0f, -11.75f, 1.0f);
    glm::vec4 T5b_b = glm::vec4(18.75f, 3.5f, -10.50f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T5b_a.x && pacman_points[i].x <= T5b_b.x &&
            pacman_points[i].y >= T5b_a.y && pacman_points[i].y <= T5b_b.y &&
            pacman_points[i].z >= T5b_a.z && pacman_points[i].z <= T5b_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= T5b_a.x && pacman_points[7].x <= T5b_b.x &&
        pacman_points[7].z < T5b_a.z && pacman_points[0].z > T5b_b.z) {
        return 1;
    }

    // colisão com T6
    glm::vec4 T6a_a = glm::vec4(17.50f, 0.0f, -6.00f, 1.0f);
    glm::vec4 T6a_b = glm::vec4(18.75f, 3.5f,  6.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T6a_a.x && pacman_points[i].x <= T6a_b.x &&
            pacman_points[i].y >= T6a_a.y && pacman_points[i].y <= T6a_b.y &&
            pacman_points[i].z >= T6a_a.z && pacman_points[i].z <= T6a_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= T6a_a.z && pacman_points[7].z <= T6a_b.z &&
        pacman_points[7].x < T6a_a.x && pacman_points[0].x > T6a_b.x) {
        return 1;
    }

    glm::vec4 T6b_a = glm::vec4(11.50f, 0.0f, -0.75f, 1.0f);
    glm::vec4 T6b_b = glm::vec4(18.75f, 3.5f, 0.50f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T6b_a.x && pacman_points[i].x <= T6b_b.x &&
            pacman_points[i].y >= T6b_a.y && pacman_points[i].y <= T6b_b.y &&
            pacman_points[i].z >= T6b_a.z && pacman_points[i].z <= T6b_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= T6b_a.x && pacman_points[7].x <= T6b_b.x &&
        pacman_points[7].z < T6b_a.z && pacman_points[0].z > T6b_b.z) {
        return 1;
    }


    // colisão com T7
    glm::vec4 T7a_a = glm::vec4(11.50f, 0.0f, 4.75f, 1.0f);
    glm::vec4 T7a_b = glm::vec4(12.75f, 3.5f, 11.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T7a_a.x && pacman_points[i].x <= T7a_b.x &&
            pacman_points[i].y >= T7a_a.y && pacman_points[i].y <= T7a_b.y &&
            pacman_points[i].z >= T7a_a.z && pacman_points[i].z <= T7a_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= T7a_a.z && pacman_points[7].z <= T7a_b.z &&
        pacman_points[7].x < T7a_a.x && pacman_points[0].x > T7a_b.x) {
        return 1;
    }

    glm::vec4 T7b_a = glm::vec4(6.50f, 0.0f, 10.50f, 1.0f);
    glm::vec4 T7b_b = glm::vec4(18.75f, 3.5f, 11.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= T7b_a.x && pacman_points[i].x <= T7b_b.x &&
            pacman_points[i].y >= T7b_a.y && pacman_points[i].y <= T7b_b.y &&
            pacman_points[i].z >= T7b_a.z && pacman_points[i].z <= T7b_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= T7b_a.x && pacman_points[7].x <= T7b_b.x &&
        pacman_points[7].z < T7b_a.z && pacman_points[0].z > T7b_b.z) {
        return 1;
    }

    // colisão com L1
    glm::vec4 L1a_a = glm::vec4(-10.25f, 0.0f, -20.75f, 1.0f);
    glm::vec4 L1a_b = glm::vec4(- 9.00f, 3.5f, -16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= L1a_a.x && pacman_points[i].x <= L1a_b.x &&
            pacman_points[i].y >= L1a_a.y && pacman_points[i].y <= L1a_b.y &&
            pacman_points[i].z >= L1a_a.z && pacman_points[i].z <= L1a_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= L1a_a.z && pacman_points[7].z <= L1a_b.z &&
        pacman_points[7].x < L1a_a.x && pacman_points[0].x > L1a_b.x) {
        return 1;
    }

    glm::vec4 L1b_a = glm::vec4(-15.75f, 0.0f, -17.25f, 1.0f);
    glm::vec4 L1b_b = glm::vec4(- 9.00f, 3.5f, -16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= L1b_a.x && pacman_points[i].x <= L1b_b.x &&
            pacman_points[i].y >= L1b_a.y && pacman_points[i].y <= L1b_b.y &&
            pacman_points[i].z >= L1b_a.z && pacman_points[i].z <= L1b_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= L1b_a.x && pacman_points[7].x <= L1b_b.x &&
        pacman_points[7].z < L1b_a.z && pacman_points[0].z > L1b_b.z) {
        return 1;
    }

    // colisão com L2
    glm::vec4 L2a_a = glm::vec4(-10.25f, 0.0f, 16.00f, 1.0f);
    glm::vec4 L2a_b = glm::vec4(- 9.00f, 3.5f, 20.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= L2a_a.x && pacman_points[i].x <= L2a_b.x &&
            pacman_points[i].y >= L2a_a.y && pacman_points[i].y <= L2a_b.y &&
            pacman_points[i].z >= L2a_a.z && pacman_points[i].z <= L2a_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= L2a_a.z && pacman_points[7].z <= L2a_b.z &&
        pacman_points[7].x < L2a_a.x && pacman_points[0].x > L2a_b.x) {
        return 1;
    }

    glm::vec4 L2b_a = glm::vec4(-15.75f, 0.0f, 16.00f, 1.0f);
    glm::vec4 L2b_b = glm::vec4(- 9.00f, 3.5f, 17.25f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= L2b_a.x && pacman_points[i].x <= L2b_b.x &&
            pacman_points[i].y >= L2b_a.y && pacman_points[i].y <= L2b_b.y &&
            pacman_points[i].z >= L2b_a.z && pacman_points[i].z <= L2b_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= L2b_a.x && pacman_points[7].x <= L2b_b.x &&
        pacman_points[7].z < L2b_a.z && pacman_points[0].z > L2b_b.z) {
        return 1;
    }

    // colisão com M1
    glm::vec4 M1_a = glm::vec4(30.00f, 0.0f, -26.25f, 1.0f);
    glm::vec4 M1_b = glm::vec4(31.25f, 3.5f,  26.25f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M1_a.x && pacman_points[i].x <= M1_b.x &&
            pacman_points[i].y >= M1_a.y && pacman_points[i].y <= M1_b.y &&
            pacman_points[i].z >= M1_a.z && pacman_points[i].z <= M1_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= M1_a.z && pacman_points[7].z <= M1_b.z &&
        pacman_points[7].x < M1_a.x && pacman_points[0].x > M1_b.x) {
        return 1;
    }

    // colisão com M2
    glm::vec4 M2_a = glm::vec4(23.25f, 0.0f, -0.75f, 1.0f);
    glm::vec4 M2_b = glm::vec4(31.25f, 3.5f,  0.50f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M2_a.x && pacman_points[i].x <= M2_b.x &&
            pacman_points[i].y >= M2_a.y && pacman_points[i].y <= M2_b.y &&
            pacman_points[i].z >= M2_a.z && pacman_points[i].z <= M2_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= M2_a.x && pacman_points[7].x <= M2_b.x &&
        pacman_points[7].z < M2_a.z && pacman_points[0].z > M2_b.z) {
        return 1;
    }

    // colisão com M3
    glm::vec4 M3_a = glm::vec4(11.50f, 0.0f, 25.00f, 1.0f);
    glm::vec4 M3_b = glm::vec4(31.25f, 3.5f, 26.25f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M3_a.x && pacman_points[i].x <= M3_b.x &&
            pacman_points[i].y >= M3_a.y && pacman_points[i].y <= M3_b.y &&
            pacman_points[i].z >= M3_a.z && pacman_points[i].z <= M3_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= M3_a.x && pacman_points[7].x <= M3_b.x &&
        pacman_points[7].z < M3_a.z && pacman_points[0].z > M3_b.z) {
        return 1;
    }

    // colisão com M4
    glm::vec4 M4_a = glm::vec4(11.50f, 0.0f, 16.00f, 1.0f);
    glm::vec4 M4_b = glm::vec4(12.75f, 3.5f, 26.25f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M4_a.x && pacman_points[i].x <= M4_b.x &&
            pacman_points[i].y >= M4_a.y && pacman_points[i].y <= M4_b.y &&
            pacman_points[i].z >= M4_a.z && pacman_points[i].z <= M4_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= M4_a.z && pacman_points[7].z <= M4_b.z &&
        pacman_points[7].x < M4_a.x && pacman_points[0].x > M4_b.x) {
        return 1;
    }

    // colisão com M5
    glm::vec4 M5_a = glm::vec4( 6.50f, 0.0f, 16.00f, 1.0f);
    glm::vec4 M5_b = glm::vec4(12.75f, 3.5f, 17.25f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M5_a.x && pacman_points[i].x <= M5_b.x &&
            pacman_points[i].y >= M5_a.y && pacman_points[i].y <= M5_b.y &&
            pacman_points[i].z >= M5_a.z && pacman_points[i].z <= M5_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= M5_a.x && pacman_points[7].x <= M5_b.x &&
        pacman_points[7].z < M5_a.z && pacman_points[0].z > M5_b.z) {
        return 1;
    }

    // colisão com M6
    glm::vec4 M6_a = glm::vec4(6.50f, 0.0f, 16.00f, 1.0f);
    glm::vec4 M6_b = glm::vec4(7.75f, 3.5f, 26.25f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M6_a.x && pacman_points[i].x <= M6_b.x &&
            pacman_points[i].y >= M6_a.y && pacman_points[i].y <= M6_b.y &&
            pacman_points[i].z >= M6_a.z && pacman_points[i].z <= M6_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= M6_a.z && pacman_points[7].z <= M6_b.z &&
        pacman_points[7].x < M6_a.x && pacman_points[0].x > M6_b.x) {
        return 1;
    }

    // colisão com M7
    glm::vec4 M7_a = glm::vec4(11.50f, 0.0f,-26.25f, 1.0f);
    glm::vec4 M7_b = glm::vec4(31.25f, 3.5f,-25.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M7_a.x && pacman_points[i].x <= M7_b.x &&
            pacman_points[i].y >= M7_a.y && pacman_points[i].y <= M7_b.y &&
            pacman_points[i].z >= M7_a.z && pacman_points[i].z <= M7_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= M7_a.x && pacman_points[7].x <= M7_b.x &&
        pacman_points[7].z < M7_a.z && pacman_points[0].z > M7_b.z) {
        return 1;
    }

    // colisão com M8
    glm::vec4 M8_a = glm::vec4(11.50f, 0.0f,-26.25f, 1.0f);
    glm::vec4 M8_b = glm::vec4(12.75f, 3.5f,-16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M8_a.x && pacman_points[i].x <= M8_b.x &&
            pacman_points[i].y >= M8_a.y && pacman_points[i].y <= M8_b.y &&
            pacman_points[i].z >= M8_a.z && pacman_points[i].z <= M8_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= M8_a.z && pacman_points[7].z <= M8_b.z &&
        pacman_points[7].x < M8_a.x && pacman_points[0].x > M8_b.x) {
        return 1;
    }

    // colisão com M9
    glm::vec4 M9_a = glm::vec4( 6.50f, 0.0f,-17.25f, 1.0f);
    glm::vec4 M9_b = glm::vec4(12.75f, 3.5f,-16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M9_a.x && pacman_points[i].x <= M9_b.x &&
            pacman_points[i].y >= M9_a.y && pacman_points[i].y <= M9_b.y &&
            pacman_points[i].z >= M9_a.z && pacman_points[i].z <= M9_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= M9_a.x && pacman_points[7].x <= M9_b.x &&
        pacman_points[7].z < M9_a.z && pacman_points[0].z > M9_b.z) {
        return 1;
    }

    // colisão com M10
    glm::vec4 M10_a = glm::vec4(6.50f, 0.0f,-26.25f, 1.0f);
    glm::vec4 M10_b = glm::vec4(7.75f, 3.5f,-16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M10_a.x && pacman_points[i].x <= M10_b.x &&
            pacman_points[i].y >= M10_a.y && pacman_points[i].y <= M10_b.y &&
            pacman_points[i].z >= M10_a.z && pacman_points[i].z <= M10_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= M10_a.z && pacman_points[7].z <= M10_b.z &&
        pacman_points[7].x < M10_a.x && pacman_points[0].x > M10_b.x) {
        return 1;
    }

    // colisão com M11
    glm::vec4 M11_a = glm::vec4(1.00f, 0.0f,-26.25f, 1.0f);
    glm::vec4 M11_b = glm::vec4(2.25f, 3.5f,-16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M11_a.x && pacman_points[i].x <= M11_b.x &&
            pacman_points[i].y >= M11_a.y && pacman_points[i].y <= M11_b.y &&
            pacman_points[i].z >= M11_a.z && pacman_points[i].z <= M11_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= M11_a.z && pacman_points[7].z <= M11_b.z &&
        pacman_points[7].x < M11_a.x && pacman_points[0].x > M11_b.x) {
        return 1;
    }

    // colisão com M12
    glm::vec4 M12_a = glm::vec4(-4.75f, 0.0f,-17.25f, 1.0f);
    glm::vec4 M12_b = glm::vec4( 2.25f, 3.5f,-16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M12_a.x && pacman_points[i].x <= M12_b.x &&
            pacman_points[i].y >= M12_a.y && pacman_points[i].y <= M12_b.y &&
            pacman_points[i].z >= M12_a.z && pacman_points[i].z <= M12_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= M12_a.x && pacman_points[7].x <= M12_b.x &&
        pacman_points[7].z < M12_a.z && pacman_points[0].z > M12_b.z) {
        return 1;
    }

    // colisão com M13
    glm::vec4 M13_a = glm::vec4(-4.75f, 0.0f,-26.25f, 1.0f);
    glm::vec4 M13_b = glm::vec4(-3.50f, 3.5f,-16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M13_a.x && pacman_points[i].x <= M13_b.x &&
            pacman_points[i].y >= M13_a.y && pacman_points[i].y <= M13_b.y &&
            pacman_points[i].z >= M13_a.z && pacman_points[i].z <= M13_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= M13_a.z && pacman_points[7].z <= M13_b.z &&
        pacman_points[7].x < M13_a.x && pacman_points[0].x > M13_b.x) {
        return 1;
    }

    // colisão com M14
    glm::vec4 M14_a = glm::vec4(-26.50f, 0.0f,-26.25f, 1.0f);
    glm::vec4 M14_b = glm::vec4(- 3.50f, 3.5f,-25.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M14_a.x && pacman_points[i].x <= M14_b.x &&
            pacman_points[i].y >= M14_a.y && pacman_points[i].y <= M14_b.y &&
            pacman_points[i].z >= M14_a.z && pacman_points[i].z <= M14_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= M14_a.x && pacman_points[7].x <= M14_b.x &&
        pacman_points[7].z < M14_a.z && pacman_points[0].z > M14_b.z) {
        return 1;
    }

    // colisão com M15
    glm::vec4 M15_a = glm::vec4(-15.75f, 0.0f,-26.25f, 1.0f);
    glm::vec4 M15_b = glm::vec4(-14.50f, 3.5f,-20.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M15_a.x && pacman_points[i].x <= M15_b.x &&
            pacman_points[i].y >= M15_a.y && pacman_points[i].y <= M15_b.y &&
            pacman_points[i].z >= M15_a.z && pacman_points[i].z <= M15_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= M15_a.z && pacman_points[7].z <= M15_b.z &&
        pacman_points[7].x < M15_a.x && pacman_points[0].x > M15_b.x) {
        return 1;
    }

    // colisão com M16
    glm::vec4 M16_a = glm::vec4(-26.50f, 0.0f,-26.25f, 1.0f);
    glm::vec4 M16_b = glm::vec4(-25.25f, 3.5f, 26.25f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M16_a.x && pacman_points[i].x <= M16_b.x &&
            pacman_points[i].y >= M16_a.y && pacman_points[i].y <= M16_b.y &&
            pacman_points[i].z >= M16_a.z && pacman_points[i].z <= M16_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= M16_a.z && pacman_points[7].z <= M16_b.z &&
        pacman_points[7].x < M16_a.x && pacman_points[0].x > M16_b.x) {
        return 1;
    }

    // colisão com M17
    glm::vec4 M17_a = glm::vec4(-26.50f, 0.0f, 25.00f, 1.0f);
    glm::vec4 M17_b = glm::vec4(- 3.50f, 3.5f, 26.25f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M17_a.x && pacman_points[i].x <= M17_b.x &&
            pacman_points[i].y >= M17_a.y && pacman_points[i].y <= M17_b.y &&
            pacman_points[i].z >= M17_a.z && pacman_points[i].z <= M17_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= M17_a.x && pacman_points[7].x <= M17_b.x &&
        pacman_points[7].z < M17_a.z && pacman_points[0].z > M17_b.z) {
        return 1;
    }

    // colisão com M18
    glm::vec4 M18_a = glm::vec4(-15.75f, 0.0f, 20.75f, 1.0f);
    glm::vec4 M18_b = glm::vec4(-14.50f, 3.5f, 26.25f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M18_a.x && pacman_points[i].x <= M18_b.x &&
            pacman_points[i].y >= M18_a.y && pacman_points[i].y <= M18_b.y &&
            pacman_points[i].z >= M18_a.z && pacman_points[i].z <= M18_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= M18_a.z && pacman_points[7].z <= M18_b.z &&
        pacman_points[7].x < M18_a.x && pacman_points[0].x > M18_b.x) {
        return 1;
    }

    // colisão com M19
    glm::vec4 M19_a = glm::vec4(-4.75f, 0.0f, 16.00f, 1.0f);
    glm::vec4 M19_b = glm::vec4(-3.50f, 3.5f, 26.25f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M19_a.x && pacman_points[i].x <= M19_b.x &&
            pacman_points[i].y >= M19_a.y && pacman_points[i].y <= M19_b.y &&
            pacman_points[i].z >= M19_a.z && pacman_points[i].z <= M19_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= M19_a.z && pacman_points[7].z <= M19_b.z &&
        pacman_points[7].x < M19_a.x && pacman_points[0].x > M19_b.x) {
        return 1;
    }

    // colisão com M20
    glm::vec4 M20_a = glm::vec4(-4.75f, 0.0f, 16.00f, 1.0f);
    glm::vec4 M20_b = glm::vec4( 2.25f, 3.5f, 17.25f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M20_a.x && pacman_points[i].x <= M20_b.x &&
            pacman_points[i].y >= M20_a.y && pacman_points[i].y <= M20_b.y &&
            pacman_points[i].z >= M20_a.z && pacman_points[i].z <= M20_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].x >= M20_a.x && pacman_points[7].x <= M20_b.x &&
        pacman_points[7].z < M20_a.z && pacman_points[0].z > M20_b.z) {
        return 1;
    }

    // colisão com M21
    glm::vec4 M21_a = glm::vec4(1.00f, 0.0f, 16.00f, 1.0f);
    glm::vec4 M21_b = glm::vec4(2.25f, 3.5f, 26.25f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (pacman_points[i].x >= M21_a.x && pacman_points[i].x <= M21_b.x &&
            pacman_points[i].y >= M21_a.y && pacman_points[i].y <= M21_b.y &&
            pacman_points[i].z >= M21_a.z && pacman_points[i].z <= M21_b.z) {
            return 1;
        }
    }
    if (pacman_points[0].z >= M21_a.z && pacman_points[7].z <= M21_b.z &&
        pacman_points[7].x < M21_a.x && pacman_points[0].x > M21_b.x) {
        return 1;
    }

    return 0;
}

// teste de colisão com os pontos
void CollisionPoints(glm::vec4 pacman_position, std::map<int, Point> *g_Points)
{
    std::map<int, Point>::iterator it  = g_Points->begin();

    // definir o pacman como uma esfera de raio r ao redor do ponto pacman_position
    double radius_pacman = 2.0f;

    // Points
    while (it != g_Points->end()) {
        Point point = it->second;

        if (!point.taken) {
            if ((pow(point.position.x - pacman_position.x, 2) +
                pow(point.position.y - pacman_position.y, 2) +
                pow(point.position.z - pacman_position.z, 2)) <= pow(radius_pacman, 2)) {
                point.taken = 1;
                g_Points->at(point.id) = point;
            }
        }
        it++;
    }
}

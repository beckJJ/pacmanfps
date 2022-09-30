#include <glm/vec4.hpp>
// teste de colisão com o labirinto
int ColisionMaze(glm::vec4 camera_position)
{
    // criar um "cubo" de tamanho 2 ao redor da câmera
    // definindo os 8 vertices do cubo
    glm::vec4 camera_points[8] = {camera_position + glm::vec4( 1.0, 1.0, 1.0,0.0f),
                                  camera_position + glm::vec4( 1.0, 1.0,-1.0,0.0f),
                                  camera_position + glm::vec4( 1.0,-1.0, 1.0,0.0f),
                                  camera_position + glm::vec4( 1.0,-1.0,-1.0,0.0f),
                                  camera_position + glm::vec4(-1.0, 1.0, 1.0,0.0f),
                                  camera_position + glm::vec4(-1.0, 1.0,-1.0,0.0f),
                                  camera_position + glm::vec4(-1.0,-1.0, 1.0,0.0f),
                                  camera_position + glm::vec4(-1.0,-1.0,-1.0,0.0f)};

    // nomes definidos conforme imagem em nomes_labirinto.png
    // colisão com R1
    glm::vec4 R1_a = glm::vec4(1.0f, 0.0f, -6.0f, 1.0f);
    glm::vec4 R1_b = glm::vec4(7.5f, 3.5f,  6.0f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= R1_a.x && camera_points[i].x <= R1_b.x &&
            camera_points[i].y >= R1_a.y && camera_points[i].y <= R1_b.y &&
            camera_points[i].z >= R1_a.z && camera_points[i].z <= R1_b.z) {
            return 1;
        }
    }

    // colisão com R2
    glm::vec4 R2_a = glm::vec4(-4.75f, 0.0f,-11.75f, 1.0f);
    glm::vec4 R2_b = glm::vec4( 2.25f, 3.5f,-10.50f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= R2_a.x && camera_points[i].x <= R2_b.x &&
            camera_points[i].y >= R2_a.y && camera_points[i].y <= R2_b.y &&
            camera_points[i].z >= R2_a.z && camera_points[i].z <= R2_b.z) {
            return 1;
        }
    }
    if (camera_points[0].x >= R2_a.x && camera_points[7].x <= R2_b.x &&
        camera_points[7].z < R2_a.z && camera_points[0].z > R2_b.z) {
        return 1;
    }

    // colisão com R3
    glm::vec4 R3_a = glm::vec4(-4.75f, 0.0f, 10.50f, 1.0f);
    glm::vec4 R3_b = glm::vec4( 2.25f, 3.5f, 11.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= R3_a.x && camera_points[i].x <= R3_b.x &&
            camera_points[i].y >= R3_a.y && camera_points[i].y <= R3_b.y &&
            camera_points[i].z >= R3_a.z && camera_points[i].z <= R3_b.z) {
            return 1;
        }
    }
    if (camera_points[0].x >= R3_a.x && camera_points[7].x <= R3_b.x &&
        camera_points[7].z < R3_a.z && camera_points[0].z > R3_b.z) {
        return 1;
    }

    // colisão com R4
    glm::vec4 R4_a = glm::vec4(-10.25f, 0.0f,-11.75f, 1.0f);
    glm::vec4 R4_b = glm::vec4( -9.00f, 3.5f, -4.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= R4_a.x && camera_points[i].x <= R4_b.x &&
            camera_points[i].y >= R4_a.y && camera_points[i].y <= R4_b.y &&
            camera_points[i].z >= R4_a.z && camera_points[i].z <= R4_b.z) {
            return 1;
        }
    }
    if (camera_points[0].z >= R4_a.z && camera_points[7].z <= R4_b.z &&
        camera_points[7].x < R4_a.x && camera_points[0].x > R4_b.x) {
        return 1;
    }

    // colisão com R5
    glm::vec4 R5_a = glm::vec4(-10.25f, 0.0f, 4.75f, 1.0f);
    glm::vec4 R5_b = glm::vec4( -9.00f, 3.5f,11.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= R5_a.x && camera_points[i].x <= R5_b.x &&
            camera_points[i].y >= R5_a.y && camera_points[i].y <= R5_b.y &&
            camera_points[i].z >= R5_a.z && camera_points[i].z <= R5_b.z) {
            return 1;
        }
    }
    if (camera_points[0].z >= R5_a.z && camera_points[7].z <= R5_b.z &&
        camera_points[7].x < R5_a.x && camera_points[0].x > R5_b.x) {
        return 1;
    }

    // colisão com R6
    glm::vec4 R6_a = glm::vec4(23.25f, 0.0f,-20.75f, 1.0f);
    glm::vec4 R6_b = glm::vec4(25.75f, 3.5f,-16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= R6_a.x && camera_points[i].x <= R6_b.x &&
            camera_points[i].y >= R6_a.y && camera_points[i].y <= R6_b.y &&
            camera_points[i].z >= R6_a.z && camera_points[i].z <= R6_b.z) {
            return 1;
        }
    }

    // colisão com R7
    glm::vec4 R7_a = glm::vec4(23.25f, 0.0f,-11.75f, 1.0f);
    glm::vec4 R7_b = glm::vec4(25.75f, 3.5f, -4.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= R7_a.x && camera_points[i].x <= R7_b.x &&
            camera_points[i].y >= R7_a.y && camera_points[i].y <= R7_b.y &&
            camera_points[i].z >= R7_a.z && camera_points[i].z <= R7_b.z) {
            return 1;
        }
    }

    // colisão com R8
    glm::vec4 R8_a = glm::vec4(17.50f, 0.0f,-20.75f, 1.0f);
    glm::vec4 R8_b = glm::vec4(18.75f, 3.5f,-16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= R8_a.x && camera_points[i].x <= R8_b.x &&
            camera_points[i].y >= R8_a.y && camera_points[i].y <= R8_b.y &&
            camera_points[i].z >= R8_a.z && camera_points[i].z <= R8_b.z) {
            return 1;
        }
    }
    if (camera_points[0].z >= R8_a.z && camera_points[7].z <= R8_b.z &&
        camera_points[7].x < R8_a.x && camera_points[0].x > R8_b.x) {
        return 1;
    }

    // colisão com R9
    glm::vec4 R9_a = glm::vec4(23.25f, 0.0f,  4.75f, 1.0f);
    glm::vec4 R9_b = glm::vec4(25.75f, 3.5f, 11.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= R9_a.x && camera_points[i].x <= R9_b.x &&
            camera_points[i].y >= R9_a.y && camera_points[i].y <= R9_b.y &&
            camera_points[i].z >= R9_a.z && camera_points[i].z <= R9_b.z) {
            return 1;
        }
    }

    // colisão com R10
    glm::vec4 R10_a = glm::vec4(23.25f, 0.0f, 16.00f, 1.0f);
    glm::vec4 R10_b = glm::vec4(25.75f, 3.5f, 20.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= R10_a.x && camera_points[i].x <= R10_b.x &&
            camera_points[i].y >= R10_a.y && camera_points[i].y <= R10_b.y &&
            camera_points[i].z >= R10_a.z && camera_points[i].z <= R10_b.z) {
            return 1;
        }
    }

    // colisão com R11
    glm::vec4 R11_a = glm::vec4(17.50f, 0.0f, 16.00f, 1.0f);
    glm::vec4 R11_b = glm::vec4(18.75f, 3.5f, 20.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= R11_a.x && camera_points[i].x <= R11_b.x &&
            camera_points[i].y >= R11_a.y && camera_points[i].y <= R11_b.y &&
            camera_points[i].z >= R11_a.z && camera_points[i].z <= R11_b.z) {
            return 1;
        }
    }
    if (camera_points[0].z >= R11_a.z && camera_points[7].z <= R11_b.z &&
        camera_points[7].x < R11_a.x && camera_points[0].x > R11_b.x) {
        return 1;
    }

    // colisão com T1
    glm::vec4 T1a_a = glm::vec4(-4.75f, 0.0f, -6.0f, 1.0f);
    glm::vec4 T1a_b = glm::vec4(-3.50f, 3.5f,  6.0f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T1a_a.x && camera_points[i].x <= T1a_b.x &&
            camera_points[i].y >= T1a_a.y && camera_points[i].y <= T1a_b.y &&
            camera_points[i].z >= T1a_a.z && camera_points[i].z <= T1a_b.z) {
            return 1;
        }
    }
    if (camera_points[0].z >= T1a_a.z && camera_points[7].z <= T1a_b.z &&
        camera_points[7].x < T1a_a.x && camera_points[0].x > T1a_b.x) {
        return 1;
    }

    glm::vec4 T1b_a = glm::vec4(-10.25f, 0.0f, -0.75f, 1.0f);
    glm::vec4 T1b_b = glm::vec4(-3.50f, 3.5f, 0.5f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T1b_a.x && camera_points[i].x <= T1b_b.x &&
            camera_points[i].y >= T1b_a.y && camera_points[i].y <= T1b_b.y &&
            camera_points[i].z >= T1b_a.z && camera_points[i].z <= T1b_b.z) {
            return 1;
        }
    }
    if (camera_points[0].x >= T1b_a.x && camera_points[7].x <= T1b_b.x &&
        camera_points[7].z < T1b_a.z && camera_points[0].z > T1b_b.z) {
        return 1;
    }

    // colisão com T2
    glm::vec4 T2a_a = glm::vec4(-15.75f, 0.0f, -6.0f, 1.0f);
    glm::vec4 T2a_b = glm::vec4(-14.50f, 3.5f,  6.0f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T2a_a.x && camera_points[i].x <= T2a_b.x &&
            camera_points[i].y >= T2a_a.y && camera_points[i].y <= T2a_b.y &&
            camera_points[i].z >= T2a_a.z && camera_points[i].z <= T2a_b.z) {
            return 1;
        }
    }
    if (camera_points[0].z >= T2a_a.z && camera_points[7].z <= T2a_b.z &&
        camera_points[7].x < T2a_a.x && camera_points[0].x > T2a_b.x) {
        return 1;
    }

    glm::vec4 T2b_a = glm::vec4(-21.50f, 0.0f, -0.75f, 1.0f);
    glm::vec4 T2b_b = glm::vec4(-14.50f, 3.5f, 0.5f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T2b_a.x && camera_points[i].x <= T2b_b.x &&
            camera_points[i].y >= T2b_a.y && camera_points[i].y <= T2b_b.y &&
            camera_points[i].z >= T2b_a.z && camera_points[i].z <= T2b_b.z) {
            return 1;
        }
    }
    if (camera_points[0].x >= T2b_a.x && camera_points[7].x <= T2b_b.x &&
        camera_points[7].z < T2b_a.z && camera_points[0].z > T2b_b.z) {
        return 1;
    }

    // colisão com T3
    glm::vec4 T3a_a = glm::vec4(-21.50f, 0.0f, -20.75f, 1.0f);
    glm::vec4 T3a_b = glm::vec4(-20.25f, 3.5f, -4.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T3a_a.x && camera_points[i].x <= T3a_b.x &&
            camera_points[i].y >= T3a_a.y && camera_points[i].y <= T3a_b.y &&
            camera_points[i].z >= T3a_a.z && camera_points[i].z <= T3a_b.z) {
            return 1;
        }
    }
    if (camera_points[0].z >= T3a_a.z && camera_points[7].z <= T3a_b.z &&
        camera_points[7].x < T3a_a.x && camera_points[0].x > T3a_b.x) {
        return 1;
    }

    glm::vec4 T3b_a = glm::vec4(-21.50f, 0.0f, -11.75f, 1.0f);
    glm::vec4 T3b_b = glm::vec4(-14.50f, 3.5f, -10.50f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T3b_a.x && camera_points[i].x <= T3b_b.x &&
            camera_points[i].y >= T3b_a.y && camera_points[i].y <= T3b_b.y &&
            camera_points[i].z >= T3b_a.z && camera_points[i].z <= T3b_b.z) {
            return 1;
        }
    }
    if (camera_points[0].x >= T3b_a.x && camera_points[7].x <= T3b_b.x &&
        camera_points[7].z < T3b_a.z && camera_points[0].z > T3b_b.z) {
        return 1;
    }

    // colisão com T4
    glm::vec4 T4a_a = glm::vec4(-21.50f, 0.0f, 4.75f, 1.0f);
    glm::vec4 T4a_b = glm::vec4(-20.25f, 3.5f, 20.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T4a_a.x && camera_points[i].x <= T4a_b.x &&
            camera_points[i].y >= T4a_a.y && camera_points[i].y <= T4a_b.y &&
            camera_points[i].z >= T4a_a.z && camera_points[i].z <= T4a_b.z) {
            return 1;
        }
    }
    if (camera_points[0].z >= T4a_a.z && camera_points[7].z <= T4a_b.z &&
        camera_points[7].x < T4a_a.x && camera_points[0].x > T4a_b.x) {
        return 1;
    }

    glm::vec4 T4b_a = glm::vec4(-21.50f, 0.0f, 10.50f, 1.0f);
    glm::vec4 T4b_b = glm::vec4(-14.50f, 3.5f, 11.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T4b_a.x && camera_points[i].x <= T4b_b.x &&
            camera_points[i].y >= T4b_a.y && camera_points[i].y <= T4b_b.y &&
            camera_points[i].z >= T4b_a.z && camera_points[i].z <= T4b_b.z) {
            return 1;
        }
    }
    if (camera_points[0].x >= T4b_a.x && camera_points[7].x <= T4b_b.x &&
        camera_points[7].z < T4b_a.z && camera_points[0].z > T4b_b.z) {
        return 1;
    }

    // colisão com T5
    glm::vec4 T5a_a = glm::vec4(11.50f, 0.0f, -11.75f, 1.0f);
    glm::vec4 T5a_b = glm::vec4(12.75f, 3.5f, -4.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T5a_a.x && camera_points[i].x <= T5a_b.x &&
            camera_points[i].y >= T5a_a.y && camera_points[i].y <= T5a_b.y &&
            camera_points[i].z >= T5a_a.z && camera_points[i].z <= T5a_b.z) {
            return 1;
        }
    }
    if (camera_points[0].z >= T5a_a.z && camera_points[7].z <= T5a_b.z &&
        camera_points[7].x < T5a_a.x && camera_points[0].x > T5a_b.x) {
        return 1;
    }

    glm::vec4 T5b_a = glm::vec4(6.50f, 0.0f, -11.75f, 1.0f);
    glm::vec4 T5b_b = glm::vec4(18.75f, 3.5f, -10.50f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T5b_a.x && camera_points[i].x <= T5b_b.x &&
            camera_points[i].y >= T5b_a.y && camera_points[i].y <= T5b_b.y &&
            camera_points[i].z >= T5b_a.z && camera_points[i].z <= T5b_b.z) {
            return 1;
        }
    }
    if (camera_points[0].x >= T5b_a.x && camera_points[7].x <= T5b_b.x &&
        camera_points[7].z < T5b_a.z && camera_points[0].z > T5b_b.z) {
        return 1;
    }

    // colisão com T6
    glm::vec4 T6a_a = glm::vec4(17.50f, 0.0f, -6.00f, 1.0f);
    glm::vec4 T6a_b = glm::vec4(18.75f, 3.5f,  6.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T6a_a.x && camera_points[i].x <= T6a_b.x &&
            camera_points[i].y >= T6a_a.y && camera_points[i].y <= T6a_b.y &&
            camera_points[i].z >= T6a_a.z && camera_points[i].z <= T6a_b.z) {
            return 1;
        }
    }
    if (camera_points[0].z >= T6a_a.z && camera_points[7].z <= T6a_b.z &&
        camera_points[7].x < T6a_a.x && camera_points[0].x > T6a_b.x) {
        return 1;
    }

    glm::vec4 T6b_a = glm::vec4(11.50f, 0.0f, -0.75f, 1.0f);
    glm::vec4 T6b_b = glm::vec4(18.75f, 3.5f, 0.50f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T6b_a.x && camera_points[i].x <= T6b_b.x &&
            camera_points[i].y >= T6b_a.y && camera_points[i].y <= T6b_b.y &&
            camera_points[i].z >= T6b_a.z && camera_points[i].z <= T6b_b.z) {
            return 1;
        }
    }
    if (camera_points[0].x >= T6b_a.x && camera_points[7].x <= T6b_b.x &&
        camera_points[7].z < T6b_a.z && camera_points[0].z > T6b_b.z) {
        return 1;
    }


    // colisão com T7
    glm::vec4 T7a_a = glm::vec4(11.50f, 0.0f, 4.75f, 1.0f);
    glm::vec4 T7a_b = glm::vec4(12.75f, 3.5f, 11.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T7a_a.x && camera_points[i].x <= T7a_b.x &&
            camera_points[i].y >= T7a_a.y && camera_points[i].y <= T7a_b.y &&
            camera_points[i].z >= T7a_a.z && camera_points[i].z <= T7a_b.z) {
            return 1;
        }
    }
    if (camera_points[0].z >= T7a_a.z && camera_points[7].z <= T7a_b.z &&
        camera_points[7].x < T7a_a.x && camera_points[0].x > T7a_b.x) {
        return 1;
    }

    glm::vec4 T7b_a = glm::vec4(6.50f, 0.0f, 10.50f, 1.0f);
    glm::vec4 T7b_b = glm::vec4(18.75f, 3.5f, 11.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= T7b_a.x && camera_points[i].x <= T7b_b.x &&
            camera_points[i].y >= T7b_a.y && camera_points[i].y <= T7b_b.y &&
            camera_points[i].z >= T7b_a.z && camera_points[i].z <= T7b_b.z) {
            return 1;
        }
    }
    if (camera_points[0].x >= T7b_a.x && camera_points[7].x <= T7b_b.x &&
        camera_points[7].z < T7b_a.z && camera_points[0].z > T7b_b.z) {
        return 1;
    }

    // colisão com L1
    glm::vec4 L1a_a = glm::vec4(-10.25f, 0.0f, -20.75f, 1.0f);
    glm::vec4 L1a_b = glm::vec4(- 9.00f, 3.5f, -16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= L1a_a.x && camera_points[i].x <= L1a_b.x &&
            camera_points[i].y >= L1a_a.y && camera_points[i].y <= L1a_b.y &&
            camera_points[i].z >= L1a_a.z && camera_points[i].z <= L1a_b.z) {
            return 1;
        }
    }
    if (camera_points[0].z >= L1a_a.z && camera_points[7].z <= L1a_b.z &&
        camera_points[7].x < L1a_a.x && camera_points[0].x > L1a_b.x) {
        return 1;
    }

    glm::vec4 L1b_a = glm::vec4(-15.75f, 0.0f, -17.25f, 1.0f);
    glm::vec4 L1b_b = glm::vec4(- 9.00f, 3.5f, -16.00f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= L1b_a.x && camera_points[i].x <= L1b_b.x &&
            camera_points[i].y >= L1b_a.y && camera_points[i].y <= L1b_b.y &&
            camera_points[i].z >= L1b_a.z && camera_points[i].z <= L1b_b.z) {
            return 1;
        }
    }
    if (camera_points[0].x >= L1b_a.x && camera_points[7].x <= L1b_b.x &&
        camera_points[7].z < L1b_a.z && camera_points[0].z > L1b_b.z) {
        return 1;
    }

    // colisão com L2
    glm::vec4 L2a_a = glm::vec4(-10.25f, 0.0f, 16.00f, 1.0f);
    glm::vec4 L2a_b = glm::vec4(- 9.00f, 3.5f, 20.75f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= L2a_a.x && camera_points[i].x <= L2a_b.x &&
            camera_points[i].y >= L2a_a.y && camera_points[i].y <= L2a_b.y &&
            camera_points[i].z >= L2a_a.z && camera_points[i].z <= L2a_b.z) {
            return 1;
        }
    }
    if (camera_points[0].z >= L2a_a.z && camera_points[7].z <= L2a_b.z &&
        camera_points[7].x < L2a_a.x && camera_points[0].x > L2a_b.x) {
        return 1;
    }

    glm::vec4 L2b_a = glm::vec4(-15.75f, 0.0f, 16.00f, 1.0f);
    glm::vec4 L2b_b = glm::vec4(- 9.00f, 3.5f, 17.25f, 1.0f);
    for (int i = 0; i < 8; i++) {
        if (camera_points[i].x >= L2b_a.x && camera_points[i].x <= L2b_b.x &&
            camera_points[i].y >= L2b_a.y && camera_points[i].y <= L2b_b.y &&
            camera_points[i].z >= L2b_a.z && camera_points[i].z <= L2b_b.z) {
            return 1;
        }
    }
    if (camera_points[0].x >= L2b_a.x && camera_points[7].x <= L2b_b.x &&
        camera_points[7].z < L2b_a.z && camera_points[0].z > L2b_b.z) {
        return 1;
    }


    return 0;
}


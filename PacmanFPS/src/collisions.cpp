#include <glm/vec4.hpp>


// teste de colisão com o labirinto
int ColisionMaze(glm::vec4 camera_position)
{
    // criar um "cubo" de tamanho 1 ao redor da câmera
    // definindo os 8 vertices do cubo
    glm::vec4 camera_points[8] = {camera_position + glm::vec4( 0.5, 0.5, 0.5,0.0f),
                                  camera_position + glm::vec4( 0.5, 0.5,-0.5,0.0f),
                                  camera_position + glm::vec4( 0.5,-0.5, 0.5,0.0f),
                                  camera_position + glm::vec4( 0.5,-0.5,-0.5,0.0f),
                                  camera_position + glm::vec4(-0.5, 0.5, 0.5,0.0f),
                                  camera_position + glm::vec4(-0.5, 0.5,-0.5,0.0f),
                                  camera_position + glm::vec4(-0.5,-0.5, 0.5,0.0f),
                                  camera_position + glm::vec4(-0.5,-0.5,-0.5,0.0f)};

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

    return 0;
}


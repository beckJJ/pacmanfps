#ifndef _STRUCTS_H
#define _STRUCTS_H

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Definimos uma estrutura que armazenará dados necessários para renderizar
// cada ghost.
struct Ghost
{
    std::string  name;           // Nome do Ghost
    int          way;            // indica o sentido que o ghost está se locomovendo
    float        delta_t;        // indica os passos que cada ghost dará no respectivo trecho do percurso
    int          checkpoint;     // indica o ultimo ponto do array de posições que o ghost chegou
    int          num_points;     // indica por quantos pontos o ghost passará na mapa
    glm::vec3    centralization; // centraliza o ghost na origem da sua coordenada local
    glm::vec4    positions[30];  // array de pontos do mapa que o ghost deverá passar
    glm::vec4    current_position; // posição atual do ghost
};

struct PointsHall
{
    int id;
    glm::vec4 hall_begin;
    glm::vec4 hall_end;
};

struct Point
{
    int       id;       // identificador do Point
    glm::vec4 position; // posição do Point no mapa
    int taken; // se o ponto foi pego pelo pacman ou não
};

struct Power
{
    int       id;
    int       way;
    float     t;
    float     steps;
    glm::vec4 actual_position;
    glm::vec4 points[4];
};

#endif // _STRUCTS_H

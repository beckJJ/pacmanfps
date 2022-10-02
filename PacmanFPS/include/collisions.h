#ifndef _COLLISIONS_H
#define _COLLISIONS_H

#include <glm/vec4.hpp>
#include <math.h>
#include <map>
#include "structs.h"

int CollisionMaze(glm::vec4 camera_position);
void CollisionPoints(glm::vec4 pacman_position, std::map<int, Point> *g_Points);
int CollisionGhostsPacman(glm::vec4 pacman_position, std::map<std::string, Ghost> g_Ghosts);
int CollisionPlanes(glm::vec4 pacman_position);
int CollisionTeleport(glm::vec4 pacman_position);
int CollisionPowerPoints(glm::vec4 pacman_position, std::map<int, Power> *g_PowerPoints);

#endif // _COLLISIONS_H


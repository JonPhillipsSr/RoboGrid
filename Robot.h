#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

// Represents a robot with a logical grid posistion and a visual shape
struct Robot
{
	int id = 0;
	int gridX = 0;
	int gridY = 0;
	sf::CircleShape shape;
};
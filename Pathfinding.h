#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Grid.h"

// Represents a single tile being evaluated during pathfinding
struct Node
{
	sf::Vector2i position;  // grid coord of this tile
	int g = 0;  // cost from start to this node
	int h = 0;  // estimated cost from this node to goal
	int f = 0;  // total score: g + h
	Node* parent = nullptr; // pointer back towards the start, used to trace final path
};

// Returns the shortest passable path from start to goal as a list of coordinates
// Returns an empty vector if no path exists.
std::vector<sf::Vector2i> findPath(
	sf::Vector2i start,
	sf::Vector2i goal,
	const std::array<std::array<TileType, gridCols>, gridRows>& worldGrid
);
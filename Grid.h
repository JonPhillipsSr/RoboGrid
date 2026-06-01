#pragma once
#include <array>

// --- constants ---
constexpr int tileSize = 40;
constexpr int windowWidth = 1200;
constexpr int windowHeight = 800;
constexpr int gridCols = windowWidth / tileSize;
constexpr int gridRows = windowHeight / tileSize;
constexpr int sidebarWidth = 200;
constexpr int totalWindowWidth = windowWidth + sidebarWidth;

// Tile types that can occupy a grid cell
enum class TileType
{
	Empty,
	Wall,
	Charger,
	PedestrianPath
};

//Return true if the target cell i in bounds and passable
bool canMoveTo(
	int targetX,
	int targetY,
	const std::array<std::array<TileType, gridCols>, gridRows>& worldGrid
);
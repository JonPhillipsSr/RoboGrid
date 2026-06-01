#include "Grid.h"

//Validates movement: rejects out of bounds positions
bool canMoveTo(
	int targetX,
	int targetY,
	const std::array<std::array<TileType, gridCols>, gridRows>& worldGrid
)
{
	if (targetX < 0 || targetX >= gridCols)
	{
		return false;
	}

	if (targetY < 0 || targetY >= gridRows)
	{
		return false;
	}

	if (worldGrid[targetY][targetX] == TileType::Wall ||
		worldGrid[targetY][targetX] == TileType::PedestrianPath)
	{
		return false;
	}

	return true;
}
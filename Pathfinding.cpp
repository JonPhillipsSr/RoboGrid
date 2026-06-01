#include "Pathfinding.h"
#include <algorithm>

std::vector<sf::Vector2i> findPath(
    sf::Vector2i start,
    sf::Vector2i goal,
    const std::array<std::array<TileType, gridCols>, gridRows>& worldGrid
)
{
    // One Node per grid cell stored in a fixed 2D array so parent pointers stay valid
    std::array<std::array<Node, gridCols>, gridRows> nodes{};
    for (int row = 0; row < gridRows; ++row)
        for (int col = 0; col < gridCols; ++col)
            nodes[row][col].position = sf::Vector2i(col, row);

    std::vector<Node*> openList;    // tiles waiting to be evaluated
    std::array<std::array<bool, gridCols>, gridRows> closedList{};  // tiles already evaluated

    // Seed the open list with the start node
    Node* startNode = &nodes[start.y][start.x];
    startNode->h = abs(start.x - goal.x) + abs(start.y - goal.y);
    startNode->f = startNode->h;
    openList.push_back(startNode);

    // The four cardinal directions a robot can move
    const sf::Vector2i directions[4] = { {0,-1}, {0,1}, {-1,0}, {1,0} };

    while (!openList.empty())
    {
        // Pick the node with the lowest f score
        auto it = std::min_element(openList.begin(), openList.end(),
            [](const Node* a, const Node* b) { return a->f < b->f; });
        Node* current = *it;
        openList.erase(it);

        closedList[current->position.y][current->position.x] = true;

        // If we reached the goal, trace the path back to the start
        if (current->position == goal)
        {
            std::vector<sf::Vector2i> path;
            Node* node = current;
            while (node != nullptr)
            {
                path.push_back(node->position);
                node = node->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Evaluate each neighbor
        for (const auto& dir : directions)
        {
            sf::Vector2i neighborPos = current->position + dir;

            // Skip out-of-bounds tiles
            if (neighborPos.x < 0 || neighborPos.x >= gridCols ||
                neighborPos.y < 0 || neighborPos.y >= gridRows)
                continue;

            // Skip already-evaluated tiles
            if (closedList[neighborPos.y][neighborPos.x])
                continue;

            // Skip impassable tiles
            if (!canMoveTo(neighborPos.x, neighborPos.y, worldGrid))
                continue;

            Node* neighbor = &nodes[neighborPos.y][neighborPos.x];
            int newG = current->g + 1;

            // If already in open list, update if this route is cheaper
            bool inOpen = false;
            for (Node* n : openList)
            {
                if (n->position == neighborPos)
                {
                    inOpen = true;
                    if (newG < n->g)
                    {
                        n->g = newG;
                        n->f = n->g + n->h;
                        n->parent = current;
                    }
                    break;
                }
            }

            // Otherwise add it to the open list
            if (!inOpen)
            {
                neighbor->g = newG;
                neighbor->h = abs(neighborPos.x - goal.x) + abs(neighborPos.y - goal.y);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;
                openList.push_back(neighbor);
            }
        }
    }

    return {}; // no path found
}
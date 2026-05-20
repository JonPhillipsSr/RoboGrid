#include <SFML/Graphics.hpp>
#include <array>

// --- Constants ---
constexpr int tileSize = 40;
constexpr int windowWidth = 1200;
constexpr int windowHeight = 800;
constexpr int gridCols = windowWidth / tileSize;    // 30
constexpr int gridRows = windowHeight / tileSize;   // 20

// Represents a robot with a logical grid position and a visual shape
struct Robot
{
    int gridX = 0;              // logical position — the true state of the robot
    int gridY = 0;
    sf::CircleShape shape;   // render-only; position is recalculated each frame
};

// Tile types that can occupy a grid cell
enum class TileType
{
    Empty,
    Wall,
    Charger,
    PedestrianPath
};

// Returns true if the target cell is in bounds and not a wall
bool canMoveTo(
    int targetX,
    int targetY,
    const std::array<std::array<TileType, gridCols>, gridRows>& worldGrid
);


int main()
{
    // Holds the logical state of every tile; default-initialized to Empty
    std::array<std::array<TileType, gridCols>, gridRows> worldGrid{};

    sf::RenderWindow window(
        sf::VideoMode({ windowWidth, windowHeight }),
        "RoboGrid"
    );

    Robot robot;
    robot.shape.setRadius(10.f);

    const sf::Color gridColor(0x3278B4FF);

    worldGrid[5][10] = TileType::Wall;  // temporary test wall

    sf::RectangleShape tile(sf::Vector2f(40.f, 40.f));

    // --- Main loop ---
    while (window.isOpen())
    {
        // --- Event handling ---
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Arrow keys move the robot one tile at a time
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                int targetX = robot.gridX;
                int targetY = robot.gridY;

                if (keyPressed->code == sf::Keyboard::Key::Right)
                {
                    targetX++;
                }
                if (keyPressed->code == sf::Keyboard::Key::Left)
                {
                    targetX--;
                }
                if (keyPressed->code == sf::Keyboard::Key::Up)
                {
                    targetY--;
                }
                if (keyPressed->code == sf::Keyboard::Key::Down)
                {
                    targetY++;
                }

                if (canMoveTo(targetX, targetY, worldGrid))
                {
                    robot.gridX = targetX;
                    robot.gridY = targetY;
                }
            }

            // Left click places a tile; right click removes it
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                int col = mousePressed->position.x / tileSize;
                int row = mousePressed->position.y / tileSize;
                bool robotIsHere = (col == robot.gridX && row == robot.gridY);

                if (!robotIsHere)
                {
                    if (col >= 0 && col < gridCols && row >= 0 && row < gridRows)
                    {
                        if (mousePressed->button == sf::Mouse::Button::Left)
                        {
                            worldGrid[row][col] = TileType::Wall;
                        }
                        if (mousePressed->button == sf::Mouse::Button::Right)
                        {
                            worldGrid[row][col] = TileType::Empty;
                        }
                    }
                }
            }
        }

        // Render position is derived from logical grid position each frame
        robot.shape.setPosition(sf::Vector2f(
            static_cast<float>(robot.gridX * tileSize + 10),
            static_cast<float>(robot.gridY * tileSize + 10)
        ));
        window.clear();

        // --- Draw tiles ---
        for (int row = 0; row < gridRows; row++)
        {
            for (int col = 0; col < gridCols; col++)
            {
                if (worldGrid[row][col] != TileType::Empty)
                {
                    switch (worldGrid[row][col])
                    {
                    case TileType::Wall:
                        tile.setFillColor(sf::Color(0xB45A28FF));
                        break;
                    case TileType::Charger:
                        tile.setFillColor(sf::Color(0x28B45AFF));
                        break;
                    case TileType::PedestrianPath:
                        tile.setFillColor(sf::Color(0xE6C800FF));
                        break;
                    default:
                        break;
                    }

                    tile.setPosition(sf::Vector2f(
                        static_cast<float>(col * tileSize),
                        static_cast<float>(row * tileSize)
                    ));
                    window.draw(tile);
                }
            }
        }

        // --- Draw grid lines ---
        // Vertical
        for (int x = 0; x < windowWidth; x += tileSize)
        {
            sf::Vertex line[] =
            {
                sf::Vertex(
                    sf::Vector2f(static_cast<float>(x), 0.f),
                    gridColor
                ),
                sf::Vertex(
                    sf::Vector2f(
                        static_cast<float>(x),
                        static_cast<float>(windowHeight)
                    ),
                    gridColor
                )
            };

            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        // Horizontal
        for (int y = 0; y < windowHeight; y += tileSize)
        {
            sf::Vertex line[] =
            {
                sf::Vertex(
                    sf::Vector2f(0.f, static_cast<float>(y)),
                    gridColor
                ),
                sf::Vertex(
                    sf::Vector2f(
                        static_cast<float>(windowWidth),
                        static_cast<float>(y)
                    ),
                    gridColor
                )
            };

            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        window.draw(robot.shape);
        window.display();
    }

    return 0;
}

// Validates movement: rejects out-of-bounds positions and wall tiles
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

    if (worldGrid[targetY][targetX] == TileType::Wall)
    {
        return false;
    }

    return true;
}

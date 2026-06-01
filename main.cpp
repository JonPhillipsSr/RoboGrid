#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Robot.h"

// Controls what happens when the user clicks the grid
enum class EditorMode
{
    PlaceTile,
    AddRobot,
    RemoveRobot
};

int main()
{
    // Holds the logical state of every tile; default-initialized to Empty
    std::array<std::array<TileType, gridCols>, gridRows> worldGrid{};

    // Load font for sidebar labels
    sf::Font font;
    if (!font.openFromFile("resources/fonts/Roboto-Regular.ttf"))
        return -1;

    TileType activeTile = TileType::Wall;           // currently selected tile type
    EditorMode activeMode = EditorMode::PlaceTile;  // currently active editor mode

    bool isPainting = false;  // true while left mouse button is held on the grid
    bool isErasing = false;  // true while right mouse button is held on the grid

    std::vector<Robot> robots;  // all active robots in the simulation
    int nextRobotId = 0;        // increments each time a robot is added

    // Paints or erases a tile at the given pixel coordinates
    auto paintTile = [&](int mouseX, int mouseY)
        {
            if (mouseX >= windowWidth)
                return; // ignore clicks in the sidebar

            int col = mouseX / tileSize;
            int row = mouseY / tileSize;

            // check if any robot occupies this tile
            bool robotIsHere = false;
            for (const auto& r : robots)
            {
                if (r.gridX == col && r.gridY == row)
                {
                    robotIsHere = true;
                    break;
                }
            }

            if (!robotIsHere && col >= 0 && col < gridCols && row >= 0 && row < gridRows)
            {
                if (isPainting)
                    worldGrid[row][col] = activeTile;
                if (isErasing)
                    worldGrid[row][col] = TileType::Empty;
            }
        };

    sf::RenderWindow window(
        sf::VideoMode({ totalWindowWidth, windowHeight }),
        "RoboGrid"
    );

    const sf::Color gridColor(0x3278B4FF);
    sf::RectangleShape tile(sf::Vector2f(40.f, 40.f));

    // --- Main loop ---
    while (window.isOpen())
    {
        // --- Event handling ---
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Left click places a tile, adds/removes a robot, or selects from sidebar
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                int mouseX = mousePressed->position.x;
                int mouseY = mousePressed->position.y;

                if (mouseX >= windowWidth)
                {
                    // Click is in the sidebar
                    if (mousePressed->button == sf::Mouse::Button::Left)
                    {
                        if (mouseY >= 20 && mouseY <= 54)
                        {
                            activeTile = TileType::Wall;
                            activeMode = EditorMode::PlaceTile;
                        }
                        else if (mouseY >= 70 && mouseY <= 104)
                        {
                            activeTile = TileType::Charger;
                            activeMode = EditorMode::PlaceTile;
                        }
                        else if (mouseY >= 120 && mouseY <= 154)
                        {
                            activeTile = TileType::PedestrianPath;
                            activeMode = EditorMode::PlaceTile;
                        }
                        // Clear grid button — grouped with tile types
                        else if (mouseY >= 170 && mouseY <= 205)
                        {
                            for (auto& row : worldGrid)
                                row.fill(TileType::Empty);
                        }
                        else if (mouseY >= 705 && mouseY <= 740)
                        {
                            activeMode = EditorMode::AddRobot;
                        }
                        else if (mouseY >= 750 && mouseY <= 785)
                        {
                            activeMode = EditorMode::RemoveRobot;
                        }
                    }
                }
                else
                {
                    // Click is in the grid
                    if (activeMode == EditorMode::PlaceTile)
                    {
                        if (mousePressed->button == sf::Mouse::Button::Left)
                            isPainting = true;
                        if (mousePressed->button == sf::Mouse::Button::Right)
                            isErasing = true;

                        paintTile(mouseX, mouseY);
                    }
                    else if (activeMode == EditorMode::AddRobot &&
                        mousePressed->button == sf::Mouse::Button::Left)
                    {
                        int col = mouseX / tileSize;
                        int row = mouseY / tileSize;

                        // check tile is passable and not already occupied by a robot
                        bool occupied = false;
                        for (const auto& r : robots)
                        {
                            if (r.gridX == col && r.gridY == row)
                            {
                                occupied = true;
                                break;
                            }
                        }

                        if (!occupied && canMoveTo(col, row, worldGrid))
                        {
                            Robot newRobot;
                            newRobot.id = nextRobotId++;
                            newRobot.gridX = col;
                            newRobot.gridY = row;
                            newRobot.shape.setRadius(10.f);
                            newRobot.shape.setFillColor(sf::Color(0x9B30FFFF));
                            robots.push_back(newRobot);
                        }
                    }
                    else if (activeMode == EditorMode::RemoveRobot &&
                        mousePressed->button == sf::Mouse::Button::Left)
                    {
                        int col = mouseX / tileSize;
                        int row = mouseY / tileSize;

                        // find and remove robot at this tile
                        for (auto it = robots.begin(); it != robots.end(); ++it)
                        {
                            if (it->gridX == col && it->gridY == row)
                            {
                                robots.erase(it);
                                break;
                            }
                        }
                    }
                }
            }

            // Release mouse button to stop painting
            if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseReleased->button == sf::Mouse::Button::Left)
                    isPainting = false;
                if (mouseReleased->button == sf::Mouse::Button::Right)
                    isErasing = false;
            }

            // Drag to paint tiles
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                if (isPainting || isErasing)
                    paintTile(mouseMoved->position.x, mouseMoved->position.y);
            }
        }

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
                sf::Vertex(sf::Vector2f(static_cast<float>(x), 0.f), gridColor),
                sf::Vertex(sf::Vector2f(static_cast<float>(x), static_cast<float>(windowHeight)), gridColor)
            };
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        // Horizontal
        for (int y = 0; y < windowHeight; y += tileSize)
        {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(0.f, static_cast<float>(y)), gridColor),
                sf::Vertex(sf::Vector2f(static_cast<float>(windowWidth), static_cast<float>(y)), gridColor)
            };
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        // --- Draw robots ---
        for (auto& r : robots)
        {
            r.shape.setPosition(sf::Vector2f(
                static_cast<float>(r.gridX * tileSize + 10),
                static_cast<float>(r.gridY * tileSize + 10)
            ));
            window.draw(r.shape);
        }

        // --- Draw sidebar ---

        // Background panel
        sf::RectangleShape sidebar(sf::Vector2f(sidebarWidth, windowHeight));
        sidebar.setPosition(sf::Vector2f(windowWidth, 0.f));
        sidebar.setFillColor(sf::Color(0x1A1A2EFF));
        window.draw(sidebar);

        // Tile swatches
        sf::RectangleShape swatch(sf::Vector2f(34.f, 34.f));

        swatch.setFillColor(sf::Color(0xB45A28FF));  // Wall
        swatch.setPosition(sf::Vector2f(windowWidth + 20.f, 20.f));
        window.draw(swatch);

        swatch.setFillColor(sf::Color(0x28B45AFF));  // Charger
        swatch.setPosition(sf::Vector2f(windowWidth + 20.f, 70.f));
        window.draw(swatch);

        swatch.setFillColor(sf::Color(0xE6C800FF));  // Pedestrian Path
        swatch.setPosition(sf::Vector2f(windowWidth + 20.f, 120.f));
        window.draw(swatch);

        // Sidebar labels
        sf::Text label(font, "", 14);
        label.setFillColor(sf::Color::White);

        label.setString("Wall");
        label.setPosition(sf::Vector2f(windowWidth + 60.f, 25.f));
        window.draw(label);

        label.setString("Charger");
        label.setPosition(sf::Vector2f(windowWidth + 60.f, 75.f));
        window.draw(label);

        label.setString("Pedestrian Path");
        label.setPosition(sf::Vector2f(windowWidth + 60.f, 125.f));
        window.draw(label);

        // Clear grid button — grouped with tile types
        sf::RectangleShape clearButton(sf::Vector2f(160.f, 35.f));
        clearButton.setFillColor(sf::Color(0x8B0000FF));
        clearButton.setPosition(sf::Vector2f(windowWidth + 20.f, 170.f));
        window.draw(clearButton);

        sf::Text clearLabel(font, "Clear Grid", 14);
        clearLabel.setFillColor(sf::Color::White);
        clearLabel.setPosition(sf::Vector2f(windowWidth + 40.f, 180.f));
        window.draw(clearLabel);

        // Add Robot button — at the bottom of the sidebar
        sf::RectangleShape addRobotBtn(sf::Vector2f(160.f, 35.f));
        addRobotBtn.setFillColor(sf::Color(0x1A6B1AFF));
        addRobotBtn.setPosition(sf::Vector2f(windowWidth + 20.f, 705.f));
        window.draw(addRobotBtn);

        sf::Text addRobotLabel(font, "Add Robot", 14);
        addRobotLabel.setFillColor(sf::Color::White);
        addRobotLabel.setPosition(sf::Vector2f(windowWidth + 40.f, 715.f));
        window.draw(addRobotLabel);

        // Remove Robot button
        sf::RectangleShape removeRobotBtn(sf::Vector2f(160.f, 35.f));
        removeRobotBtn.setFillColor(sf::Color(0x8B4500FF));
        removeRobotBtn.setPosition(sf::Vector2f(windowWidth + 20.f, 750.f));
        window.draw(removeRobotBtn);

        sf::Text removeRobotLabel(font, "Remove Robot", 14);
        removeRobotLabel.setFillColor(sf::Color::White);
        removeRobotLabel.setPosition(sf::Vector2f(windowWidth + 35.f, 760.f));
        window.draw(removeRobotLabel);

        // Highlight around the active selection
        sf::RectangleShape highlight;
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(sf::Color::White);
        highlight.setOutlineThickness(2.f);

        if (activeMode == EditorMode::PlaceTile)
        {
            highlight.setSize(sf::Vector2f(38.f, 38.f));
            if (activeTile == TileType::Wall)
                highlight.setPosition(sf::Vector2f(windowWidth + 18.f, 18.f));
            else if (activeTile == TileType::Charger)
                highlight.setPosition(sf::Vector2f(windowWidth + 18.f, 68.f));
            else if (activeTile == TileType::PedestrianPath)
                highlight.setPosition(sf::Vector2f(windowWidth + 18.f, 118.f));
        }
        else if (activeMode == EditorMode::AddRobot)
        {
            highlight.setSize(sf::Vector2f(164.f, 39.f));
            highlight.setPosition(sf::Vector2f(windowWidth + 18.f, 703.f));
        }
        else if (activeMode == EditorMode::RemoveRobot)
        {
            highlight.setSize(sf::Vector2f(164.f, 39.f));
            highlight.setPosition(sf::Vector2f(windowWidth + 18.f, 748.f));
        }

        window.draw(highlight);
        window.display();
    }

    return 0;
}
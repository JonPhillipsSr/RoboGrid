# RoboGrid

## Overview

RoboGrid is a C++20 and SFML-based factory robot route-planning and simulation project currently in active development.

The long-term goal of the project is to create a simulation environment where users can:

* Design warehouse and factory floor layouts
* Place autonomous robots
* Define robot pathways and work zones
* Simulate robot traffic and routing
* Manage charging stations and battery systems
* Create pedestrian walkways and crosswalk systems
* Analyze robot efficiency and congestion
* Experiment with factory automation strategies before implementing them in the real world

The project is intentionally being built incrementally in order to focus on learning:

* Modern C++
* Software architecture
* Simulation systems
* Grid-based pathfinding
* Rendering systems
* Event-driven programming
* Object-oriented design
* Multithreading
* Git workflows
* Eventually AI-assisted analysis and planning

---

# Technologies Used

* C++20
* SFML 3.1.0
* Visual Studio 2022
* Git / GitHub

---

# Current Features

## Rendering System

* SFML render window
* Main render loop
* Event polling system
* Configurable tile-based grid rendering
* Custom color styling using 32-bit hexadecimal colors

## Grid System

* Tile-based world architecture
* Configurable tile size
* Grid dimensions automatically derived from window size
* World state stored in a 2D array of `TileType`
* Global `constexpr` constants for compile-time grid dimensions

## Robot System

* Multiple robots supported via `std::vector<Robot>`
* Each robot has a unique ID assigned at creation
* Logical grid coordinates strictly separated from render coordinates
* Robot shape rendered as a circle, centered within tiles
* Robots store a planned path and current path index for autonomous movement
* Robots cannot occupy the same tile as another robot

## Tile Types

Current implemented tile types:

* Empty
* Wall
* Charger
* Pedestrian Path

## Tile Editor

* Left click to place the selected tile type
* Click and drag to paint multiple tiles at once
* Right click to erase tiles back to Empty
* Cannot place tiles on any robot's current position
* Clear Grid button resets all tiles to Empty

## Sidebar UI

* Tile type selector with colored swatches and text labels
* Add Robot / Remove Robot buttons
* Set Destination mode button
* Clear Grid button grouped with tile tools
* Active selection highlight with adaptive size (swatch vs button)
* Text labels rendered using Roboto font (bundled in project resources)

## Collision and Movement System

* Robots cannot leave the simulation boundaries
* Robots cannot move into wall tiles
* Robots cannot move into pedestrian path tiles
* All movement validated through a reusable `canMoveTo()` function in `Grid.cpp`

## Pathfinding System

* A* pathfinding implemented in `Pathfinding.h` / `Pathfinding.cpp`
* Uses Manhattan distance as the heuristic
* Finds the shortest passable route between two grid positions
* Returns an empty path if no route exists
* Robots move autonomously along their assigned path using an `sf::Clock` timer
* Users assign destinations via the Set Destination sidebar mode:
  * First click selects a robot
  * Second click sets the destination and triggers pathfinding

---

# Current Project Architecture

RoboGrid maintains a strict separation between simulation state and rendering state.

## Simulation State

The true logical state of the world:

* Robot grid coordinates
* Tile types
* Pathfinding data

## Rendering State

The visual representation of that data:

* SFML shapes
* Grid line rendering
* Sidebar UI

This separation supports future systems such as:

* AI behavior
* Networking
* Save/load systems
* Multi-robot coordination
* Threaded simulation systems

## File Structure

| File | Purpose |
|---|---|
| `main.cpp` | Window, event loop, rendering, sidebar UI |
| `Grid.h` / `Grid.cpp` | TileType enum, grid constants, `canMoveTo()` |
| `Robot.h` | Robot struct definition |
| `Pathfinding.h` / `Pathfinding.cpp` | A* pathfinding algorithm |

---

# Planned Features

## Robot Systems

* Battery systems
* Charger prioritization and seeking behavior
* Traffic management and collision avoidance
* Robot state machines
* Autonomous task scheduling
* Robot naming for identification

## Pathfinding

* Route optimization
* Congestion-aware routing
* Dynamic rerouting when paths are blocked
* Visual path display on the grid

## Simulation Features

* Adjustable simulation speed
* Performance metrics
* Traffic analysis
* Charger utilization tracking
* Robot efficiency statistics
* Simulation reports

## UI / Editor Features

* Zoom and camera controls
* Simulation play/pause/reset controls
* Configurable simulation settings
* Save and load floor plans

## AI Integration (Long-Term Goal)

Future AI-assisted features may include:

* Natural language factory commands
* AI-generated layout suggestions
* Simulation analytics
* Bottleneck analysis
* Operational optimization recommendations
* Conversational factory assistant

Potential future integrations:

* Google Gemini
* OpenAI APIs

---

# Learning Goals

This project is also intended to serve as a personal learning and portfolio project focused on:

* Real-world C++ architecture
* Simulation design
* Professional Git workflows
* Branching and merging strategies
* Incremental software development
* Long-term maintainable code structure

---

# Current Development Status

Current implementation includes:

* Multi-file C++ architecture (Grid, Robot, Pathfinding, main)
* Tile-based grid with multiple tile types and distinct colors
* Interactive tile editor with click and drag painting
* Dynamic robot placement and removal via sidebar
* A* pathfinding with autonomous robot movement along assigned paths
* Sidebar UI with tile selection, robot management, and destination setting
* Strict separation of simulation state from render state
* All movement validated through a centralized `canMoveTo()` function

The project has a working simulation foundation with autonomous robot pathfinding. Current development is focused on expanding robot behavior systems, including battery management and multi-robot coordination.

---

# License

This project is currently a personal educational and portfolio project.

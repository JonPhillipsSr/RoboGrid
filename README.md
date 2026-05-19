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
* World state stored in a 2D array

## Robot System

* Robot represented as a dedicated struct
* Logical robot coordinates separated from rendering coordinates
* Grid-based movement using arrow keys
* Boundary collision prevention
* Robot rendering centered within tiles

## Tile Types

Current implemented tile types:

* Empty
* Wall

## Collision System

* Robot cannot leave the simulation boundaries
* Robot collision against wall tiles

---

# Current Project Architecture

One of the major architectural goals of RoboGrid is maintaining a strict separation between:

## Simulation State

The true logical state of the world:

* Robot grid coordinates
* Tile types
* Simulation data

## Rendering State

The visual representation of that data:

* SFML shapes
* Grid rendering
* Visual overlays

This separation is intentional because it supports future systems such as:

* Pathfinding
* AI behavior
* Networking
* Save/load systems
* Multi-robot coordination
* Threaded simulation systems

---

# Planned Features

## World Systems

* Additional tile types
* Obstacles
* Restricted zones
* Pedestrian pathways
* Crosswalk systems
* Workstations
* Delivery zones
* Dynamic map editing

## Robot Systems

* Multiple robots
* Robot task assignment
* Battery systems
* Charger prioritization
* Traffic management
* Collision avoidance
* State machines
* Autonomous task scheduling

## Pathfinding

Planned pathfinding systems include:

* A* pathfinding
* Route optimization
* Congestion-aware routing
* Dynamic rerouting

## Simulation Features

* Adjustable simulation speed
* Performance metrics
* Traffic analysis
* Charger utilization tracking
* Robot efficiency statistics
* Simulation reports

## UI / Editor Features

* Interactive floor plan editor
* Tile painting tools
* Simulation controls
* Robot placement tools
* Zoom and camera controls
* Configurable simulation settings

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

# Planned Refactoring

The project currently exists primarily inside `main.cpp` while foundational systems are being learned and developed.

As the project grows, the architecture will be modularized into separate files and classes.

Planned future structure:

* Robot class
* Grid class
* Simulation manager
* Charging manager
* Pathfinding systems
* UI systems

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

Current prototype supports:

* Grid rendering
* Robot movement
* World tiles
* Wall collision

The project is currently transitioning from a simple rendering prototype into a true simulation architecture.

---

# License

This project is currently a personal educational and portfolio project.

# OpenMiner

[![Build Status](https://travis-ci.com/Unarelith/OpenMiner.svg?branch=master)](https://travis-ci.com/Unarelith/OpenMiner)
[![Documentation Status](https://readthedocs.org/projects/openminer/badge/?version=latest)](https://openminer.readthedocs.io/en/latest/?badge=latest)
[![Documentation](https://codedocs.xyz/Quent42340/OpenMiner.svg)](https://codedocs.xyz/Quent42340/OpenMiner/)
[![License](https://img.shields.io/badge/license-LGPLv2.1%2B-blue.svg)](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html)
[![Discord](https://img.shields.io/discord/527527086756200458.svg?style=popout)](https://discord.gg/eN8k8wt)
[![Donate](https://img.shields.io/badge/donate-paypal-brightgreen.svg)](https://www.paypal.me/Unarelith)

- [Project goal](#project-goal)
- [Wiki](#wiki)
- [Keys](#keys)
- [How to compile](#how-to-compile)
    - [Using Windows](#using-windows)
    - [Using Linux](#using-linux)
    - [Using MacOS](#using-macos)
- [Discussion](#discussion)
- [Project status](#project-status)
- [Screenshots](#screenshots)
- [Credits](#credits)

## Project goal

The long-term goal of this project is to provide a viable alternative to Minecraft modding.

## Wiki

- [Getting started](https://github.com/Unarelith/OpenMiner/wiki/Getting-Started)
    - [Contributing](https://github.com/Unarelith/OpenMiner/blob/master/CONTRIBUTING.md)
    - [Coding style](https://github.com/Unarelith/OpenMiner/wiki/Coding-Style)
- [Lua API](https://openminer.readthedocs.io/en/latest/)
    - [Core API](https://openminer.readthedocs.io/en/latest/lua-api-core/)
    - [Mod API](https://openminer.readthedocs.io/en/latest/lua-api-mod/)
    - [GUI API](https://openminer.readthedocs.io/en/latest/lua-api-gui/)
    - [C++ classes in Lua](https://openminer.readthedocs.io/en/latest/lua-api-cpp/)
- [Network Protocol](https://openminer.readthedocs.io/en/latest/network-protocol/)

## Keys

- Movement: <kbd>W</kbd><kbd>A</kbd><kbd>S</kbd><kbd>D</kbd>
- Inventory: <kbd>E</kbd>
- Creative window: <kbd>H</kbd>
- Chat: <kbd>T</kbd>
- Jump: <kbd>Space</kbd>
- Sprint: <kbd>Ctrl</kbd>
- Jetpack: <kbd>X</kbd>
- Exit: <kbd>Escape</kbd>
- Debug: <kbd>F3</kbd>
- Fullscreen: <kbd>F11</kbd>

## How to compile

### Using Windows

- [Wiki: Compiling on Windows using CMake and MinGW-w64](https://github.com/Unarelith/OpenMiner/wiki/Compiling-on-Windows-with-MinGW-w64)
- [Wiki: Compiling on Windows using CMake and Visual Studio 2019](https://github.com/Unarelith/OpenMiner/wiki/Compiling-on-Windows-with-Visual-Studio-2019)

### Using Linux

- [Wiki: Compiling on Linux](https://github.com/Unarelith/OpenMiner/wiki/Compiling-on-Linux)

### Using MacOS

- Dependencies:
    - A compiler with C++17 support (GCC >= 7.0 or clang >= 5.0)
    - `git` and [CMake](http://www.cmake.org/download/) (>= 3.12.4)
    - **Optional:** `ninja` (much faster builds)
    - _Check Homebrew for packages._
- Run `cmake -B build . && cmake --build build`; if you installed `ninja` above, add `-GNinja` before `-B` in the previous command.
- Run the client with `./build/openminer`
- If you want a multiplayer game, run the server with `./build/openminer_server`

## Discussion

- Discord: [join server](https://discord.gg/eN8k8wt)
- IRC: irc.freenode.net `#openminer`

## Project status

This list is non exhaustive.

### Implemented features

- Menus (title screen, server selection, pause, settings)
- Basic worldgen (biomes, lakes, trees, flowers, pseudo-caves)
- Infinite world in X/Y/Z axis ([#25](https://github.com/Unarelith/OpenMiner/pull/25))
- Smooth lighting
- Networking ([#20](https://github.com/Unarelith/OpenMiner/pull/20))
- Chat system with support for commands ([#57](https://github.com/Unarelith/OpenMiner/pull/57))
- Lua API
	- Block/item/recipe/biome/tree/sky/dimension/entity definition
	- Custom GUI creation
- Special blocks (workbench, furnace)
- Block metadata
- Player model display (without animation)
- Dimensions (like the Nether or the Ender in Minecraft) ([#80](https://github.com/Unarelith/OpenMiner/pull/80))
- World loading/saving (see [#26](https://github.com/Unarelith/OpenMiner/issues/26))
- Texture pack system (partially implemented, see [#34](https://github.com/Unarelith/OpenMiner/issues/34))
- Entities ([#90](https://github.com/Unarelith/OpenMiner/pull/90))

### Missing features

- Fluid propagation ([#62](https://github.com/Unarelith/OpenMiner/issues/62))
- Day/night cycle with sun/moon display ([#73](https://github.com/Unarelith/OpenMiner/issues/73))
- Seed-based worldgen ([#116](https://github.com/Unarelith/OpenMiner/issues/116))
- Cave tunnels ([#118](https://github.com/Unarelith/OpenMiner/issues/118))
- Clouds ([#52](https://github.com/Unarelith/OpenMiner/pull/52))
- Particle system

## Screenshots

![](screenshot1.png?raw=true)
![](screenshot2.png?raw=true)
![](screenshot3.png?raw=true)
![](screenshot4.png?raw=true)
![](screenshot5.png?raw=true)
![](screenshot6.png?raw=true)

## Credits

- xMrVizzy ([Faithful 32x](https://www.curseforge.com/minecraft/texture-packs/faithful-32x) texture pack, will completely replace Vanilla textures)
- ThePhD ([sol2](https://github.com/ThePhD/sol2))
- skypjack ([EnTT](https://github.com/skypjack/entt))


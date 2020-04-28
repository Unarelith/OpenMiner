# OpenMiner

[![Build Status](https://travis-ci.com/Unarelith/OpenMiner.svg?branch=master)](https://travis-ci.com/Unarelith/OpenMiner)
[![Documentation Status](https://readthedocs.org/projects/openminer/badge/?version=latest)](https://openminer.readthedocs.io/en/latest/?badge=latest)
[![Documentation](https://codedocs.xyz/Quent42340/OpenMiner.svg)](https://codedocs.xyz/Quent42340/OpenMiner/)
[![License](https://img.shields.io/badge/license-LGPLv2.1%2B-blue.svg)](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html)
[![Discord](https://img.shields.io/discord/527527086756200458.svg?style=popout)](https://discord.gg/eN8k8wt)
[![Donate](https://img.shields.io/badge/donate-paypal-brightgreen.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=B4DQHF3YL4KV2&source=url)

- [Project goal](#project-goal)
- [Wiki](#wiki)
- [Keys](#keys)
- [How to compile](#how-to-compile)
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
        - [Block](https://openminer.readthedocs.io/en/latest/lua-api-block/)
        - [Item](https://openminer.readthedocs.io/en/latest/lua-api-item/)
        - [Recipe](https://openminer.readthedocs.io/en/latest/lua-api-recipe/)
        - [Sky](https://openminer.readthedocs.io/en/latest/lua-api-sky/)
        - [Tree](https://openminer.readthedocs.io/en/latest/lua-api-tree/)
        - [Biome](https://openminer.readthedocs.io/en/latest/lua-api-biome/)
        - [Dimension](https://openminer.readthedocs.io/en/latest/lua-api-dimension/)
    - [GUI API](https://openminer.readthedocs.io/en/latest/lua-api-gui/)
        - [Crafting widget](https://openminer.readthedocs.io/en/latest/lua-api-gui-crafting/)
        - [Image](https://openminer.readthedocs.io/en/latest/lua-api-gui-image/)
        - [Inventory data](https://openminer.readthedocs.io/en/latest/lua-api-gui-inventory-data/)
        - [Inventory widget](https://openminer.readthedocs.io/en/latest/lua-api-gui-inventory/)
        - [Progress bar](https://openminer.readthedocs.io/en/latest/lua-api-gui-progress-bar/)
        - [Scroll bar](https://openminer.readthedocs.io/en/latest/lua-api-gui-scroll-bar/)
    - [C++ classes in Lua](https://openminer.readthedocs.io/en/latest/lua-api-cpp/)
- [Network Protocol](https://github.com/Unarelith/OpenMiner/wiki/Network-Protocol)

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

### Using Linux

- Dependencies:
    - A compiler with C++17 support (GCC >= 7.0 or clang >= 5.0)
    - [CMake](http://www.cmake.org/download/)
    - [GameKit](http://github.com/Unarelith/GameKit) (requires `SDL2` + `tinyxml2`, will switch to `SFML` starting from 2.6)
    - [SFML](https://www.sfml-dev.org/) (only used for network)
    - [Lua](http://www.lua.org)
    - _Linux users: Check your distribution repositories for packages._
- Run `cmake -B build . && cmake --build build -j8`
- Or `mkdir build && cd build && cmake .. && make -j8 && cmake ..` (for old CMake versions)
- Run the client with `./build/openminer`
- If you want a multiplayer game, run the server with `./build/openminer_server`

### Using Windows

- [Wiki: Compiling on Windows using CMake and MinGW-w64](https://github.com/Unarelith/OpenMiner/wiki/Compiling-on-Windows-with-MinGW-w64)
- [Wiki: Compiling on Windows using CMake and Visual Studio 2017](https://github.com/Unarelith/OpenMiner/wiki/Compiling-on-Windows-with-Visual-Studio-2017)

## Discussion

- Discord: [join server](https://discord.gg/eN8k8wt)
- IRC: irc.freenode.net `#openminer`

## Project status

This list is non exhaustive.

### Implemented features

- Menus (title screen, server selection, pause menu, settings menu)
- Basic worldgen (biomes, lakes, trees, flowers, pseudo-caves)
- Infinite world in X/Y/Z axis ([#25](https://github.com/Unarelith/OpenMiner/pull/25))
- Lighting system with smooth lighting
- Networking ([#20](https://github.com/Unarelith/OpenMiner/pull/20))
- Chat system with support for commands ([#57](https://github.com/Unarelith/OpenMiner/pull/57))
- Lua API
	- Block/item/recipe/biome/tree/sky/dimension definition
	- Custom GUI creation
- Special blocks (workbench, furnace)
- Block metadata
- Player model display (currently without rotation nor animation)
- Dimensions (like the Nether or the Ender in Minecraft) ([#80](https://github.com/Unarelith/OpenMiner/pull/80))
- World loading/saving (using `/save <name>` and `/load <name>` commands, see [#26](https://github.com/Unarelith/OpenMiner/issues/26))
- Texture pack system (partially implemented, see [#34](https://github.com/Unarelith/OpenMiner/issues/34))
- Entities (block drops, mobs, etc...) ([#90](https://github.com/Unarelith/OpenMiner/pull/90))

### Missing features

- Fluid propagation ([#62](https://github.com/Unarelith/OpenMiner/issues/62))
- Day/night cycle with sun/moon display ([#73](https://github.com/Unarelith/OpenMiner/issues/73))
- Real worldgen (seed-based, cave tunnels) ([#79](https://github.com/Unarelith/OpenMiner/issues/79))
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


# OpenMiner

[![Build Status](https://travis-ci.com/Unarelith/OpenMiner.svg?branch=master)](https://travis-ci.com/Unarelith/OpenMiner)
[![Documentation](https://codedocs.xyz/Quent42340/OpenMiner.svg)](https://codedocs.xyz/Quent42340/OpenMiner/)
[![License](https://img.shields.io/badge/license-LGPLv2.1%2B-blue.svg)](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html)
[![Discord](https://img.shields.io/discord/527527086756200458.svg?style=popout)](https://discord.gg/eN8k8wt)
[![Donate](https://img.shields.io/badge/donate-paypal-brightgreen.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=66DH462V7TA6N&source=url)

- [Wiki](#wiki)
- [Project goal](#project-goal)
- [Screenshots](#screenshots)
- [Keys](#keys)
- [How to compile](#how-to-compile)
- [Discussion](#discussion)
- [Project status](#project-status)
- [Previous attempts](#previous-attempts)
- [Credits](#credits)

## Wiki

- [Lua API Overview](https://github.com/Unarelith/OpenMiner/wiki/Lua-API-Overview)

## Project goal

The long-term goal of this project is to provide a viable alternative to Minecraft modding.

## Screenshots

![](screenshot1.png?raw=true)
![](screenshot2.png?raw=true)
![](screenshot3.png?raw=true)

## Keys

- Movement: <kbd>W</kbd><kbd>A</kbd><kbd>S</kbd><kbd>D</kbd>
- Inventory: <kbd>E</kbd>
- Jump: <kbd>Space</kbd>
- Sprint: <kbd>Ctrl</kbd>
- Jetpack: <kbd>X</kbd>
- Exit: <kbd>Escape</kbd>

## How to compile

- Dependencies:
    - A compiler with C++14 support
    - [CMake](http://www.cmake.org/download/)
    - [GameKit](http://github.com/Unarelith/GameKit) (requires `SDL2` + `tinyxml2`, will switch to `SFML` starting from 2.6)
    - [SFML](https://www.sfml-dev.org/) (only used for network)
    - [Lua](http://www.lua.org)
    - _Linux users: Check your distribution repositories for packages._
- Run `cmake . && make -j8`
- Run the server with `./server/openminer_server`
- Run the client with `./client/openminer`

## Discussion

- Discord: [join server](https://discord.gg/eN8k8wt)
- IRC: irc.freenode.net `#openminer`

## Project status

This list is non exhaustive.

### Terrain generation

- [x] Basic terrain gen
- [x] Trees
- [x] Flowers
- [ ] Tall grass
- [ ] Biomes
- [ ] Mountains
- [ ] Cave tunnels

### Lighting

- [x] Basic lighting system
- [x] Cheap ambient occlusion
- [x] Smooth lighting
- [x] Sunlight downward propagation ~(possible but laggy)~
- [x] Ability to block light ~(possible but causes weird AO effects)~

### Network

- [x] World
- [x] Custom GUI
- [x] Inventories
- [x] Players

### Lua API

- [x] Register blocks/craft/items in the game
- [x] Create custom GUI and show them to player when rightclicking a block

### World

- [x] Block ticking (Furnace)
- [x] Mining speed/capability
- [ ] World loading/saving
- [ ] Block metadata (currently using a `u16` instead of the real thing)
- [ ] Fluid propagation

### Menus

- [x] Pause menu
- [x] Settings menu
- [x] Title screen
- [x] Server selection menu

## Previous attempts

- Here is the [commit](https://github.com/Unarelith/OpenMiner/tree/8eba845421efff6ce941f8550ff79e6364970fd5) matching the end of the first attempt made in 2013.
- And here is the [commit](https://github.com/Unarelith/OpenMiner/tree/58c23a7e66404dab94e51998a179dc370c89ea06) matching the end of the second attempt made in 2014.

## Credits

- MadPixel (Minecrafter font) [CC BY-ND]
- xMrVizzy ([Faithful 32x](https://www.curseforge.com/minecraft/texture-packs/faithful-32x) texture pack, will completely replace Vanilla textures)


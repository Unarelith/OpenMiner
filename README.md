# OpenMiner

[![Build Status](https://travis-ci.com/Quent42340/OpenMiner.svg?branch=network)](https://travis-ci.com/Quent42340/OpenMiner)
[![Documentation](https://codedocs.xyz/Quent42340/OpenMiner.svg)](https://codedocs.xyz/Quent42340/OpenMiner/)
[![License](https://img.shields.io/badge/license-LGPLv2.1%2B-blue.svg)](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html)
[![Discord](https://img.shields.io/discord/527527086756200458.svg?style=popout)](https://discord.gg/eN8k8wt)

- OpenMiner is a Minecraft-like game engine.

![](screenshot.png?raw=true)

## Keys

- Movement: <kbd>W</kbd><kbd>A</kbd><kbd>S</kbd><kbd>D</kbd>
- Inventory: <kbd>E</kbd>
- Jump: <kbd>Space</kbd>
- Jetpack: <kbd>X</kbd>
- Exit: <kbd>Escape</kbd>

## How to compile

- Dependencies:
    - [CMake](http://www.cmake.org/download/)
    - [GameKit](http://github.com/Quent42340/GameKit) (requires `SDL2` + `tinyxml2`, will switch to `SFML` starting from 2.6)
    - [SFML](https://www.sfml-dev.org/) (only used for network)
    - [Lua](http://www.lua.org)
    - _Linux users: Check your distribution repositories for packages._
- Run `mkdir build && cd build` at the root of the game folder
- Run `cmake .. && make`, wait (or use `-j8`), run the game and enjoy!

## Discussion

- Discord: [join server](https://discord.gg/eN8k8wt)
- IRC: irc.freenode.net `#openminer`

## Previous versions

- Here is the [commit](https://github.com/Quent42340/OpenMiner/tree/8eba845421efff6ce941f8550ff79e6364970fd5) matching the end of the first version made in 2013.
- And here is the [commit](https://github.com/Quent42340/OpenMiner/tree/58c23a7e66404dab94e51998a179dc370c89ea06) matching the end of the rewrite made in 2014.

## Credits

- MadPixel (Minecrafter font) [CC BY-ND]
- xMrVizzy ([Faithful 32x](https://www.curseforge.com/minecraft/texture-packs/faithful-32x) texture pack)


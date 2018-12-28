# OpenMiner

### By Unarelith

- OpenMiner is a Minecraft-like game.

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
    - [SDL2](https://www.libsdl.org/download-2.0.php), [SDL2_image](https://www.libsdl.org/projects/SDL_image/), [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)
    - OpenGL >= 2.1, [glm](http://sourceforge.net/projects/ogl-math/files/latest/download?source=files)
    - [glew](http://sourceforge.net/projects/glew/files/latest/download) or [glad](https://github.com/Dav1dde/glad) *(only required for Windows)*
    - [tinyxml2](http://leethomason.github.io/tinyxml2/)
    - _Linux users: Check your distribution repositories for packages._
- Run `mkdir build && cd build` at the root of the game folder
- Run `cmake .. && make`, wait (or use `-j8`), run the game and enjoy!

## Discussion

- IRC: irc.freenode.net `#openminer`
- Discord: [join server](https://discord.gg/eN8k8wt)

## Previous versions

- Here is the [commit](https://github.com/Quent42340/KubKraft/tree/8eba845421efff6ce941f8550ff79e6364970fd5) matching the end of the first version made in 2013.
- And here is the [commit](https://github.com/Quent42340/KubKraft/tree/58c23a7e66404dab94e51998a179dc370c89ea06) matching the end of the rewrite made in 2014.


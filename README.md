# SpriteBounceSDL
A simple program using SDL in c++ as an example.

## Usage
### macOS/Linux
Compile with `make` and run with `./SpriteBounce`
### Windows
Compile with the Visual C++ Project and run the resulting exe file.

## Getting SDL
This project uses SDL2 and SDL2_image.
Download them from https://www.libsdl.org/download-2.0.php and https://www.libsdl.org/projects/SDL_image/.
### macOS
Choose runtime binaries for macOS, and place the framework in `/Library/Frameworks`
### Linux
You will have to get SDL from your package manager e.g. `sudo apt install libsdl2-dev libsdl2-image-dev`
### Windows
Download the development libraries for Visual C++, and update the project to have the include and library directories. Also move the DLLs into the same directory as the exe.

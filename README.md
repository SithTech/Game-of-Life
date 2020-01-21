# Game-of-Life
A simple simulation of Conway's Game of Life.

# Compile and Run
This project uses C++, OpenGL, and GLUT
It has been tested on an Apple Macbook running macOS 10.15.2

While in the projects directory, run the following command from the terminal
* `make crr`

This command from the Makefile
* Clears any remaining objects files
* Compiles in release mode (smaller and applies compiler optimizations)
* Runs the executable

# Controls
* [Spacebar] 
  * Starts and stops the simulation
* [r] 
  * Resets the simulation with a random distribution of cells
* [c] 
  * Clears the simulation buffer
* [+]/[-] 
  * Adds/Subtracts 1 to/from the draw size (Holding [Shift] along with [+]/[-] changes the amount by 10)
* [Left Click] 
  * Spawns a square of cells at the location of the mouse click
  * Note: This pauses the simulation. Restart the simulation by hitting [Spacebar]


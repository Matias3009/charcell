# Charcell2
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Proyecto para visualizar y guardar la medición de una celda de carga.

## Requirements
Libraries:  
- libsdl2 (SDL2)
- sdl2-ttf (SDL2-TTF)

Build tools:  
- cmake
- make (normally in build-essential or included in the distro/OS)
- git

To use serialports:  
- (Linux) User in dialout group (normally required)

## How to build (Linux)

Clone the repository:  
  
`$ git clone --recurse-submodules https://github.com/Matias3009/charcell.git`

Go into it:

`$ cd charcell/`

Make the executable:

`$ make`

It will create a `build/` directory with the executable, but you can run:  

`$ make run`

And it will make the executable (if it's not compiled yet) and run it.

# sort_visualization

A sorting algorithm visualizer written in C++ with SFML.

<!-- TODO: add a screenshot or GIF (docs/screenshot.png). The Linux build works, but no screenshot was captured yet. -->

## What

- 16 sorting algorithms.
- 3 ways to fill the array before sorting: ascending, descending, random. The program also shuffles the array between runs.
- 5 layouts: columns, points, circumference, spiral, pyramid.
- 5 colour themes.
- An option to set the order in which the sorting algorithms run.

## Controls

| Key | Action |
| --- | --- |
| Up / Down, W / S | Move in the menu |
| Left / Right, A / D | Change a setting or table cell |
| Enter | Select |
| Right / D during a sort | Speed up |
| Left / A during a sort | Slow down (back to normal speed) |
| Esc | Back to the menu |

Known detail: Esc during a sort restarts the menu with a recursive call to `main()`. This is kept as is.

## Build

You need a C++17 compiler, CMake 3.10 or newer, and SFML 2.x (graphics, window, system).

Linux (Debian or Ubuntu):

```sh
sudo apt install build-essential cmake libsfml-dev
cmake -B build
cmake --build build
cd build && ./sort_visualization
```

The program loads `font/Ruda-Medium.ttf` from the working directory. CMake copies the font next to the executable, so run the program from `build/`.

Windows: install SFML 2.x, point CMake to it with `-DSFML_DIR=<path>/lib/cmake/SFML`, and build with Visual Studio or MinGW. Put the SFML DLLs next to the executable. This repository no longer contains pre-built binaries or DLLs.

## Stack

- C++17
- SFML 2.5 or newer (tested with 2.6.2)
- CMake

## Status

Finished in 2022. No new features are planned.

## License

MIT, see `LICENSE`. The font `Code/font/Ruda-Medium.ttf` is Ruda under the SIL Open Font License 1.1, see `Code/font/OFL.txt`.

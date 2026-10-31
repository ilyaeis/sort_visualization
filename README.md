# sort_visualization

A sorting algorithm visualizer in C++ with SFML: 16 algorithms, 5 ways to draw the array.

![Five short clips from the program: Quick sort as columns, Merge sort as points, Heap sort on a circle, Radix sort as a spiral, and Shell sort as a pyramid. Each clip shows the sorted array, a shuffle, and the sort.](docs/hero.gif)

All images on this page are frames saved by the program itself (see [Record](#record)).

## Run

### Download

Get the zip for your system from the [Releases page](https://github.com/ilyaeis/sort_visualization/releases/latest) (`linux-x64` or `windows-x64`), unzip it and start `sort_visualization` (`sort_visualization.exe` on Windows). The font is built into the program, and SFML is linked in, so nothing else needs to be installed. On Linux you need a desktop session with OpenGL.

### Build from source

You need a C++17 compiler and CMake 3.16 or newer. CMake downloads and builds SFML 2.6.2 for you (static), so you do not install SFML.

```sh
git clone https://github.com/ilyaeis/sort_visualization.git
cd sort_visualization
cmake -B build && cmake --build build
./build/sort_visualization          # Windows: build\Debug\sort_visualization.exe or build\Release\...
```

On Debian or Ubuntu, install the build tools and the system libraries that SFML needs once:

```sh
sudo apt install build-essential cmake git libx11-dev libxrandr-dev libxcursor-dev libudev-dev libfreetype-dev libgl1-mesa-dev
```

On Windows, use Visual Studio (with the C++ workload) or MinGW; the same two `cmake` commands work. For a faster program on Windows with Visual Studio, add `--config Release` to the build command.

You can run the program from any folder.

## What it does

- 16 sorting algorithms on an array of 500 values.
- 3 ways to fill the array before sorting: ascending, descending, random. The program also shuffles the array between runs.
- 5 layouts: columns, points, circle (the menu calls it "Circumference"), spiral, pyramid.
- 5 colour themes.
- An option to set the order in which the sorting algorithms run.
- The two values that the algorithm touches in a step get a highlight colour.

![The five layouts in the middle of a sort, side by side: columns, points, circle, spiral and pyramid.](docs/layouts.png)

## How it works

1. **Menu.** You pick a layout, and in the settings a theme, the fill order and the order of the algorithms.
2. **Fill.** The menu fills the array (ascending by default).
3. **Shuffle.** Before each algorithm, a shuffle mixes the array. The shuffle is drawn too.
4. **Sort.** The algorithms run one after another. Each algorithm calls `drawing()` once per step (a compare, a swap or a write) with the two indexes it works on.
5. **Draw.** `drawing()` asks `ArrayDrawer` to draw the array. The drawer maps each value onto the chosen layout: a column height, a point height, a position on a circle, a spiral radius, or an ellipse size in the pyramid. In the circle, spiral and pyramid layouts the colour also comes from the value.

At normal speed every step is drawn. When you speed up, only every n-th step is drawn.

Code: `Code/Main.cpp` (main loop, `drawing()`, record mode), `Code/Menu.cpp`, `Code/SortAlgorithms.cpp`, `Code/ShuffleAlgorithms.cpp`, `Code/ArrayDrawer.cpp` (layouts).

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

## Record

Record mode saves one run as PNG frames. It skips the menu and does not open a window: it draws into an offscreen texture. It runs one clip (sorted array, shuffle, sort, sorted array) and exits. Without `SORTVIS_RECORD` the program works as before.

| Variable | Meaning | Default |
| --- | --- | --- |
| `SORTVIS_RECORD` | Folder for the frames (`f000000.png`, ...). Turns record mode on. The folder must exist. | off |
| `SORTVIS_LAYOUT` | `columns`, `points`, `circle`, `spiral` or `pyramid` | `columns` |
| `SORTVIS_SORT` | Algorithm name as in the menu table, for example `Quick`, `Odd-Even` | `Quick` |
| `SORTVIS_FRAMES` | About how many frames the sort gets. The shuffle gets 1/6 of this. | `300` |
| `SORTVIS_THEME` | Colour theme, 0 to 4 | `0` |
| `SORTVIS_SEED` | Random seed for the shuffle | `1` |

The program first runs the algorithm once without drawing to count its steps. Then it saves one frame every (steps / frames) steps. So a slow algorithm and a fast one both give about the same number of frames.

```sh
mkdir -p frames
SORTVIS_RECORD=frames SORTVIS_LAYOUT=spiral SORTVIS_SORT=Radix SORTVIS_FRAMES=60 ./build/sort_visualization
```

SFML still needs a graphics context, so on Linux it needs a running X or Wayland session, but no window appears.

To make a GIF from the frames (this is how `docs/hero.gif` was made, one clip per layout, joined):

```sh
ffmpeg -framerate 15 -i frames/f%06d.png \
  -vf "scale=480:-1:flags=lanczos,split[a][b];[a]palettegen=max_colors=64:stats_mode=diff[p];[b][p]paletteuse=dither=none:diff_mode=rectangle" \
  clip.gif
```

## Web build

The sorting and shuffle algorithms also build to WebAssembly. `Code/SortAlgorithms.cpp` and `Code/ShuffleAlgorithms.cpp` compile unchanged. Without SFML, `Code/AnotherFunc.hpp` leaves out the SFML parts (when `__EMSCRIPTEN__` or `SORTVIS_NO_SFML` is defined).

`web/shim.cpp` takes the place of `Main.cpp` and the menu:

- Its `drawing()` does not draw. It records the steps, like record mode: a dry run counts the steps, then the real run saves every n-th step.
- It has the same `rand()` as glibc, so a seed gives the same shuffle as the Linux program.
- It fills the array like the menu: 500 values, ascending, descending or random.

The log is a list of 32-bit integers: `[total steps, stride, frame count]`, then for each frame `first, second, k` and `k` pairs `(index, value)`: the cells that changed since the frame before. The last frame has `first = second = -1` and shows the sorted array.

To build, install [emsdk](https://emscripten.org/docs/getting_started/downloads.html), then:

```sh
./build_web.sh    # writes web/dist/sortvis.js and web/dist/sortvis.wasm
```

The [CI](.github/workflows/build.yml) builds it too (artifact `sortvis-web`). The page at https://ilyaeis.github.io/apps/sorting/ uses it. The drawing there is a JavaScript port of `ArrayDrawer.cpp`.

## Algorithms

Each strip shows, from left to right: the shuffled array, the sort at about 25 %, 50 % and 75 % of its steps, and the sorted array. Columns layout, theme 0, seed 1.

### Bubble

Swaps neighbours that are in the wrong order. The large values move to the end first.

![Bubble sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/bubble.png)

### Insertion

Takes the next value and moves it left into the sorted part.

![Insertion sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/insertion.png)

### Gnome

Like insertion sort, but it moves one swap at a time, forward and back.

![Gnome sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/gnome.png)

### Selection

Finds the smallest value in the unsorted part and puts it at the front.

![Selection sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/selection.png)

### Odd-Even

Compares pairs at odd positions, then pairs at even positions, until no swap happens.

![Odd-Even sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/odd-even.png)

### Count

Counts how often each value occurs, then writes the values back in order.

![Count sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/count.png)

### Pigeonhole

Puts each value into its own hole, then reads the holes in order.

![Pigeonhole sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/pigeonhole.png)

### Radix

Sorts by the last digit, then the next digit, and so on (base 10).

![Radix sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/radix.png)

### Comb

Bubble sort with a gap that gets smaller each pass.

![Comb sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/comb.png)

### Shell

Insertion sort on values a gap apart, with smaller gaps each pass.

![Shell sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/shell.png)

### Merge

Sorts both halves, then merges them.

![Merge sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/merge.png)

### Quick

Splits the array round a pivot, then sorts each part.

![Quick sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/quick.png)

### Pancake

Flips the front of the array to bring the largest value to its place.

![Pancake sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/pancake.png)

### Cocktail

Bubble sort in both directions.

![Cocktail sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/cocktail.png)

### Heap

Builds a max-heap, then moves the top to the end, again and again.

![Heap sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/heap.png)

### Stooge

Sorts the first 2/3, the last 2/3, then the first 2/3 again. It is very slow.

![Stooge sort in the columns layout: shuffled array, then three steps of the sort, then the sorted array.](docs/algorithms/stooge.png)

## Stack

- C++17
- SFML 2.6.2 (downloaded by CMake, linked static)
- CMake
- Emscripten (web build only)

## Status

A school project, finished in 2022. No new features are planned. In 2026 a record mode and this README with images were added.

## License

MIT, see `LICENSE`. The font `Code/font/Ruda-Medium.ttf` is Ruda under the SIL Open Font License 1.1, see `Code/font/OFL.txt`.

Part of my portfolio: https://ilyaeis.github.io/?go=sorting

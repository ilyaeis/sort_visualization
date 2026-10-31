// Web build glue. It replaces Main.cpp, Menu.cpp and AnotherFunc.cpp (they need SFML).
// The algorithms in SortAlgorithms.cpp and ShuffleAlgorithms.cpp compile unchanged.
// drawing() records the steps into a log that JavaScript reads.
#include "../Code/SortAlgorithms.hpp"
#include "../Code/ShuffleAlgorithms.hpp"
#include <cstdint>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

// ---- rand() as in glibc (TYPE_3), so a seed gives the same shuffle as the Linux build.
struct Rng { uint32_t r[34]; int pos; };
static Rng rng;
extern "C" int rand() {
    int p = rng.pos;                                              // slot of r[i]
    rng.pos = (p + 1) % 34;
    rng.r[p] = rng.r[(p + 3) % 34] + rng.r[(p + 31) % 34];        // r[i-31] + r[i-3]
    return int(rng.r[p] >> 1);
}
extern "C" void srand(unsigned seed) {
    int32_t s = int32_t(seed ? seed : 1);
    rng.r[0] = uint32_t(s);
    for (int i = 1; i < 31; i++) {
        int64_t v = (16807LL * int32_t(rng.r[i - 1])) % 2147483647;
        rng.r[i] = uint32_t(int32_t(v < 0 ? v + 2147483647 : v));
    }
    for (int i = 31; i < 34; i++) rng.r[i] = rng.r[i - 31];
    rng.pos = 0;                                                  // r[34] goes to slot 0
    for (int i = 0; i < 310; i++) rand();                         // glibc drops these
}

// ---- helper from AnotherFunc.cpp that radix sort needs
int find_index_of_max(std::vector<int> array) {
    int remember = 0;
    for (int i = 0; i < (int)array.size(); i++) if (array[i] > array[remember]) remember = i;
    return remember;
}

// ---- recording
static std::vector<int> arr, prev, logv;  // the array, the last recorded state, the log
static long steps = 0, stride = 1;
static bool counting = false;
static int frames = 0;

// Log one frame: the two indexes, then the cells that changed since the last frame.
static void frame(const std::vector<int>& a, int first, int second) {
    logv.push_back(first);
    logv.push_back(second);
    size_t kpos = logv.size();
    logv.push_back(0);
    for (int i = 0; i < (int)a.size(); i++) {
        if (a[i] == prev[i]) continue;
        logv.push_back(i);
        logv.push_back(a[i]);
        prev[i] = a[i];
        logv[kpos]++;
    }
    frames++;
}

// Called by the algorithms once per step, before the swap or write of that step.
void drawing(std::vector<int>* array, int, int first, int second) {
    if (!counting && steps % stride == 0) frame(*array, first, second);
    steps++;
}

// Like plan_frames() in Main.cpp: a dry run on a copy counts the steps,
// then the real run records every stride-th step. Returns the log length.
template <typename F> static int record(F run, int max_frames) {
    Rng saved = rng;
    std::vector<int> copy = arr;
    counting = true; steps = 0;
    run(&copy);
    long total = steps;
    rng = saved;                                                  // real run gets the same numbers
    long m = max_frames > 0 ? max_frames : 1;
    stride = total > m ? (total + m - 1) / m : 1;
    counting = false; steps = 0; frames = 0;
    prev = arr;
    logv.assign({ int(total), int(stride), 0 });
    run(&arr);
    frame(arr, -1, -1);                                           // last frame: the final array
    logv[2] = frames;
    return (int)logv.size();
}

extern "C" {
EMSCRIPTEN_KEEPALIVE void sv_srand(unsigned seed) { srand(seed); }
EMSCRIPTEN_KEEPALIVE int sv_rand() { return rand(); }

// 0 ascending, 1 descending, 2 random. Same values as Menu::array_filler (500 items, size 2).
EMSCRIPTEN_KEEPALIVE int sv_fill(int order) {
    const int n = 500, size = 2, max_num = 1000;
    arr.clear();
    for (int i = 0; i < n; i++) arr.push_back(order == 2 ? rand() % max_num + 1 : (i + 1) * size);
    if (order == 1) for (int i = 0; i < n / 2; i++) swap(&arr[i], &arr[n - i - 1]);
    return n;
}

EMSCRIPTEN_KEEPALIVE int sv_gen_shuffle(int start) { return generate_shuffle(start != 0).type_of_shuffle; }

EMSCRIPTEN_KEEPALIVE int sv_shuffle(int type, int max_frames) {
    if (type < 0 || type > 2) return -1;                          // the original exits here
    Shuffle s{};
    s.type_of_shuffle = type;
    return record([&](std::vector<int>* a) { start_shuffle(a, s); }, max_frames);
}

EMSCRIPTEN_KEEPALIVE int sv_sort(const char* name, int max_frames) {
    std::vector<std::string> names = get_sorts_table_origin();
    std::string n = name;
    if (std::find(names.begin(), names.end(), n) == names.end()) return -1;  // the original exits here
    return record([&](std::vector<int>* a) { start_right_sort(a, n); }, max_frames);
}

EMSCRIPTEN_KEEPALIVE int* sv_log() { return logv.data(); }
EMSCRIPTEN_KEEPALIVE int* sv_array() { return arr.data(); }
EMSCRIPTEN_KEEPALIVE int sv_length() { return (int)arr.size(); }
}

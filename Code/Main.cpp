#include "Menu.hpp"
#include "ShuffleAlgorithms.hpp"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>

// ---------------- const values
int main();
int const screen_width = 1000;
int const screen_height = 1000;

int const vector_lenght = screen_height / 2;
float const vector_one_item_size = screen_width / vector_lenght;

int const faster_const = vector_lenght / 50;

int faster;
int const max_speed = vector_lenght;
bool draw = true;

// Created in main(). Record mode draws offscreen and never opens it.
sf::RenderWindow window;

sf::Time long_sleep = sf::seconds(2);
sf::Time short_sleep = sf::seconds(0.1);

ArrayDrawer drawer(screen_width, screen_height, vector_one_item_size, max_speed);
Menu menu(screen_width, screen_height);

bool key_press_for_visualizer(int draw) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				drawer.set_sort_name("");
				main();
				exit(1);
				
			}
			if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
			{
				if (draw == true) {
					faster = faster_const;
				} else if (faster < max_speed) {
					faster += faster_const;
				}
				drawer.set_speed(faster);
				return false;
			}
			if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
			{
				if (faster != faster_const) {
					faster = faster_const;
				}
				drawer.set_speed(1);
				return true;
			}
		}
	}
	return draw * 1;
}

// ---------------- record mode (see "Record" in README.md)
// SORTVIS_RECORD=<dir> turns it on. The program then draws into an
// offscreen texture, saves frames as <dir>/f000000.png ..., and exits.
struct Recorder {
	bool on = false;
	bool counting = false;       // dry run: count steps, draw nothing
	long steps = 0, every = 1;   // save a frame every `every` steps
	int frame = 0;
	std::string dir;
	sf::RenderTexture canvas;
	ArrayDrawer drawer{ screen_width, screen_height, int(vector_one_item_size), 0 };  // 0: no speed line
} rec;

void record_frame(const std::vector<int>& array, int first_column, int second_column) {
	rec.drawer.draw_array(rec.canvas, array, first_column, second_column, 1);
	char path[1024];
	std::snprintf(path, sizeof path, "%s/f%06d.png", rec.dir.c_str(), rec.frame++);
	rec.canvas.getTexture().copyToImage().saveToFile(path);
}

void drawing(std::vector<int> *array, int i, int first_column, int second_column) {
	if (rec.on) {
		if (!rec.counting && rec.steps % rec.every == 0) record_frame(*array, first_column, second_column);
		rec.steps++;
		return;
	}
	draw = key_press_for_visualizer(draw);
	if (draw || i % faster == 0) {
		drawer.draw_array(window, *array, first_column, second_column, 1);
	}
}

// ---------------- main func 
void sort_visualization(){
	std::vector<std::string> sort_order = menu.get_sort_table();
	
	std::vector<Shuffle> shuffle_order = { generate_shuffle(true) };
	for (int i = 1; i < sort_order.size(); i++) {
		shuffle_order.push_back(generate_shuffle(false));
	}

	std::vector<int> array = menu.get_array();

	draw = true;
	faster = faster_const;
	drawer.set_speed(1);

	for (int i = 0; i < sort_order.size(); i++) {

		drawer.draw_array(window, array, -1, -1, 1);
		sf::sleep(long_sleep);

		drawer.set_sort_name("Shuffle");
		start_shuffle(&array, shuffle_order[i]);
		
		drawer.draw_array(window, array, -1, -1, 1);
		sf::sleep(long_sleep);

		draw = true;
		faster = faster_const;
		drawer.set_speed(1);

		drawer.set_sort_name(sort_order[i] + " sort");
		start_right_sort(&array, sort_order[i]);

		drawer.draw_array(window, array, -1, -1, 1);
		sf::sleep(long_sleep);
	}	
	sf::sleep(long_sleep);
}

int env_int(const char* name, int fallback) {
	const char* v = std::getenv(name);
	return v ? std::atoi(v) : fallback;
}

// Runs `step` once without drawing to count its steps, then sets the
// frame step so that the real run saves about `frames` frames.
template <typename F>
void plan_frames(std::vector<int> array, int frames, F step) {
	rec.counting = true;
	rec.steps = 0;
	step(&array);
	rec.counting = false;
	rec.every = rec.steps / frames > 1 ? rec.steps / frames : 1;
	rec.steps = 0;
}

// One clip: sorted array -> shuffle -> sort -> sorted array.
int record() {
	std::vector<std::string> layouts = { "columns", "points", "circle", "spiral", "pyramid" };
	const char* layout = std::getenv("SORTVIS_LAYOUT");
	int choice = 1;
	for (int i = 0; layout && i < layouts.size(); i++) if (layouts[i] == layout) choice = i + 1;

	const char* sort_env = std::getenv("SORTVIS_SORT");
	std::string sort = sort_env ? sort_env : "Quick";
	std::vector<std::string> sorts = get_sorts_table_origin();
	if (std::find(sorts.begin(), sorts.end(), sort) == sorts.end()) {
		std::cerr << "Unknown SORTVIS_SORT: " << sort << "\n";
		return 1;
	}
	int frames = env_int("SORTVIS_FRAMES", 300);
	srand(env_int("SORTVIS_SEED", 1));

	if (!rec.canvas.create(screen_width, screen_height)) return 1;
	menu.set_theme(env_int("SORTVIS_THEME", 0));
	std::vector<sf::Color> colors = menu.get_colors();
	rec.drawer.set_colors(colors[0], colors[2], colors[3], colors[4]);
	rec.drawer.set_choice(choice);

	std::vector<int> array = menu.get_array();
	record_frame(array, -1, -1);

	unsigned seed = rand();
	auto shuffle = [](std::vector<int>* a) { random_shuffle_vector(a, true); };
	srand(seed); plan_frames(array, frames / 6, shuffle);
	srand(seed); rec.drawer.set_sort_name("Shuffle"); shuffle(&array);
	record_frame(array, -1, -1);

	int sort_start = rec.frame;
	auto sorter = [&sort](std::vector<int>* a) { start_right_sort(a, sort); };
	seed = rand();
	srand(seed); plan_frames(array, frames, sorter);
	srand(seed); rec.drawer.set_sort_name(sort + " sort"); sorter(&array);
	record_frame(array, -1, -1);

	std::cout << rec.frame << " frames, sort starts at frame " << sort_start
	          << ", 1 sort frame every " << rec.every << " steps\n";
	return 0;
}

int main()
{
	static bool started = false;
	if (!started) {
		started = true;
		const char* dir = std::getenv("SORTVIS_RECORD");
		if (dir) {
			rec.on = true;
			rec.dir = dir;
			return record();
		}
		window.create(sf::VideoMode(screen_width, screen_height), "Sort visualization");
	}

	srand(time(NULL));

	while (true) {
		drawer.set_choice(menu.start(window));

		std::vector<sf::Color> colors = menu.get_colors();
		drawer.set_colors(colors[0], colors[2], colors[3], colors[4]);

		sort_visualization();
	}

    return 0;
}

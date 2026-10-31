#ifndef ARRAYDRAWER_HPP_INCLUDED
#define ARRAYDRAWER_HPP_INCLUDED

#include "AnotherFunc.hpp"

class ArrayDrawer {
private:
	float const pi = 3.14;

	int main_text_size, sub_text_size;

	int screen_width, screen_height;
	float vector_one_item_size;
	int choice = 1;
	int speed = 1, max_speed;

	std::string sort_name;
	sf::Color column_color, background_color, main_column_color, sub_column_color;
	sf::Font font = get_font();

public:
	ArrayDrawer(int screen_width, int screen_height, int vector_one_item_size, int max_speed);

	void draw_colums(sf::RenderTarget& window, std::vector<int> array, int main_column, int sub_column);
	void draw_points(sf::RenderTarget& window, std::vector<int> array, int main_column, int sub_column);
	void draw_circumference(sf::RenderTarget& window, std::vector<int> array, int main_column, int sub_column);
	void draw_spiral(sf::RenderTarget& window, std::vector<int> array, int main_column, int sub_column);
	void draw_pyramid(sf::RenderTarget& window, std::vector<int> array, int main_column, int sub_column);

	void draw_array( sf::RenderTarget& window, std::vector<int> array, int main_column, int sub_column, bool reset_screen);
	void set_sort_name(std::string sort_name);
	void set_choice(int choice);
	void set_speed(int speed);
	void set_colors(sf::Color column_color, sf::Color background_color, sf::Color main_column_color, sf::Color sub_column_color);
};

#endif

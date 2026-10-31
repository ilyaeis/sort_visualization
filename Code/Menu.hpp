#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

#include "ArrayDrawer.hpp"
#include "SortAlgorithms.hpp"

class Menu {
private:
	int screen_width, screen_height;

	int main_text_size, sub_text_size, all_menu_text_size;
	int start_of_text_in_menu, start_of_text_in_settings;
	
	int menu_right_now_choice = 0;

	std::vector<std::string> menu_text = { "Menu", "Column sort", "Points sort", "Circumference sort", "Spiral sort", "Pyramid sort", "Settings", "Exit"};
	int main_menu_choice = 1;

	std::vector<std::string> settings_text = { "Settings", "Themes", "How Fill Array", "Sort algorithms order", "Exit"};
	int settings_choice = 1;
	//												   text and column color,         selected text color            backgraund color               main column color              sub column color
	std::vector< std::vector<sf::Color> > themes = { { sf::Color(255,183,  3), sf::Color(251,133,  0), sf::Color(  2, 48, 71), sf::Color(247, 37,133), sf::Color(114,  9,183) },
													 { sf::Color(141,153,174), sf::Color(237,242,244), sf::Color( 43, 45, 66), sf::Color(239, 35, 60), sf::Color(217,  4, 41) },
													 { sf::Color(138,201, 38), sf::Color( 25,130,196), sf::Color(106, 76,147), sf::Color(255, 89, 94), sf::Color(255,202, 58) },
													 { sf::Color(255,255,255), sf::Color(225,229,242), sf::Color(  2, 43, 58), sf::Color(249,233,236), sf::Color(191,215,255) },
													 { sf::Color(157, 78,221), sf::Color(199,125,255), sf::Color( 16,  0, 43), sf::Color(199,125,255), sf::Color(224,170,255) } };
	int themes_choice = rand() % themes.size();

	std::vector<std::string> ways_of_filling_array = { "Ascending order", "Descending order", "Random order"};
	int how_fill_array_choice = 0;

	std::vector <std::string> sorts_table = get_sorts_table_origin();
	int sort_choice = 0, remember_sort_choice = -1;
	int table_elements_in_row = 4;

	std::vector<int> array;

	int circle_size = menu_text.size() * 40;
	int vector_lenght;
	float vector_one_element_size;

	sf::Color simple_color, selected_color, background_color;
	sf::Font font = get_font();

public:
	Menu(int screen_width, int screen_height);
	std::string check_event(sf::RenderWindow& window);
	void array_filler();

	void sort_algorithms_order(sf::RenderWindow& window, ArrayDrawer& drawer);
	void how_fill_array(sf::RenderWindow& window, ArrayDrawer& drawer);
	void change_theme(sf::RenderWindow& window, ArrayDrawer& drawer);

	void settings_menu(sf::RenderWindow& window, ArrayDrawer& drawer);

	void main_menu(sf::RenderWindow& window, ArrayDrawer& drawer);

	int start(sf::RenderWindow& window);

	std::vector<sf::Color> get_colors();
	void set_theme(int theme);
	std::vector<int> get_array();
	std::vector<std::string> get_sort_table();
};

#endif

#pragma once

#if !defined(__EMSCRIPTEN__) && !defined(SORTVIS_NO_SFML)
#include <SFML/Graphics.hpp>
#else
// Web build (web/shim.cpp) and native tests: no SFML.
// Include only what the sort and shuffle code needs.
#include <string>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#endif
#include <vector>
#include <math.h>

void drawing(std::vector<int>* array, int i, int first_column, int second_column);

#if !defined(__EMSCRIPTEN__) && !defined(SORTVIS_NO_SFML)
sf::RectangleShape get_rectangle(int x, int y, int width, int height, sf::Color color);
sf::ConvexShape get_convex_shape(std::vector<float>  cords, sf::Color color, sf::Color outline_color, int outline_size);
sf::Vertex* get_line(float x1, float y1, float x2, float y2, sf::Color color);
sf::CircleShape get_ellipse(float Ox, float Oy, float x, float y, float width, sf::Color color);
sf::CircleShape get_circle(float radius, float x, float y, sf::Color color);

sf::Text get_text(std::string text_line, int size, int x, int y, int screen_width, int screen_height, sf::Color color, sf::Color outline_color, sf::Font& font);
sf::Font get_font();

sf::Color HSL_to_RGB(float h, float s, float l);
#endif

int find_index_of_max(std::vector<int> array);
int index_of_num_next_after(int num, std::vector<int> array);

void check_if_number_is_less_or_greater(int& num, int smallest, int biggest);
int find_max_len(std::vector<std::string> array);

template <typename T> 
void swap(T* first, T* second) {
    T temp = *first;
    *first = *second;
    *second = temp;
}

void fill_vector_in_ascending_order(std::vector<int>* array, int lenght, int vector_one_item_size);
void fill_vector_with_random_nums(std::vector<int>* array, int lenght, int vector_one_item_size, int max_num);
void fill_vector_in_descending_order(std::vector<int>* array, int lenght, int vector_one_item_size);

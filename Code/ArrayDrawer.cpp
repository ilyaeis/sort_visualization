#include "ArrayDrawer.hpp"
#include <iostream>
ArrayDrawer::ArrayDrawer(int screen_width, int screen_height, int vector_one_item_size, int max_speed) {
    this->screen_width = screen_width;
    this->screen_height = screen_height;

    this->vector_one_item_size = vector_one_item_size;

    this->max_speed = max_speed;
    this->column_color = column_color;
    this->background_color = background_color;
    this->main_column_color = main_column_color;
    this->sub_column_color = sub_column_color;

    main_text_size = screen_height / 25;
    sub_text_size = screen_height / 50;
}

void ArrayDrawer::draw_colums(sf::RenderTarget& window, std::vector<int> array, int main_column, int sub_column) {
    for (int i = 0; i < array.size(); i++) {
        if (i == main_column) {
            window.draw(get_rectangle(i * vector_one_item_size, 0, 1 * vector_one_item_size, array[i], main_column_color));
        }
        else if (i == sub_column) {
            window.draw(get_rectangle(i * vector_one_item_size, 0, 1 * vector_one_item_size, array[i], sub_column_color));
        }
        else {
            window.draw(get_rectangle(i * vector_one_item_size, 0, 1 * vector_one_item_size, array[i], column_color));
        }
    }
}

void ArrayDrawer::draw_points(sf::RenderTarget& window, std::vector<int> array, int main_column, int sub_column) {
    for (int i = 0; i < array.size(); i++) {
        if (i == main_column) {
            window.draw(get_circle(vector_one_item_size * 2, i * vector_one_item_size - vector_one_item_size, screen_height - array[i] - vector_one_item_size, main_column_color));
        }
        else if (i == sub_column) {
            window.draw(get_circle(vector_one_item_size * 2, i * vector_one_item_size - vector_one_item_size, screen_height - array[i] - vector_one_item_size, sub_column_color));
        }
        else {
            window.draw(get_circle(vector_one_item_size * 2, i * vector_one_item_size - vector_one_item_size, screen_height - array[i] - vector_one_item_size, column_color));
        }
    }
}

void ArrayDrawer::draw_circumference(sf::RenderTarget& window, std::vector<int> array, int main_column, int sub_column) {

    float radius_for_circle_of_dots = screen_height / 3;
    float radius_for_dots = screen_height / array.size();
    float pi = 3.14;

    float color_for_element = 360.0 / screen_height;
    float angel_per_element = 360.0 / array.size() * pi / 180.0;

    int Ox = screen_width / 2;
    int Oy = screen_height / 2;

    int first_dot = index_of_num_next_after(-1, array);
    for (int i = 0; i < array.size(); i++) {
        int second_dot = index_of_num_next_after(array[first_dot], array);
        
        float x1 = radius_for_circle_of_dots * cos((first_dot)*angel_per_element);
        float y1 = radius_for_circle_of_dots * sin((first_dot)*angel_per_element);

        float x2 = radius_for_circle_of_dots * cos((second_dot)*angel_per_element);
        float y2 = radius_for_circle_of_dots * sin((second_dot)*angel_per_element);

        sf::Color first_dot_color = HSL_to_RGB(color_for_element * (array[first_dot] - 1), 1.0, 0.5);
        sf::Color second_dot_color = HSL_to_RGB(color_for_element * (array[second_dot] - 1), 1.0, 0.5);
        sf::Color line_color = first_dot_color;

        if (first_dot == main_column || first_dot == sub_column) {
            first_dot_color = main_column_color;
            line_color = main_column_color;
        }
        else if (second_dot == main_column || second_dot == sub_column) {
            second_dot_color = main_column_color;
        }

        sf::Vertex line[2];
        line[0].position = sf::Vector2f(Ox + x1, Oy + y1);
        line[0].color = line_color;
        line[1].position = sf::Vector2f(Ox + x2, Oy + y2);
        line[1].color = line_color;

        window.draw(get_circle(radius_for_dots, Ox + x1, Oy + y1, first_dot_color));
        window.draw(get_circle(radius_for_dots, Ox + x2, Oy + y2, second_dot_color));
        window.draw(line, 2, sf::Lines);

        first_dot = second_dot;
    }
}

void ArrayDrawer::draw_spiral(sf::RenderTarget& window, std::vector<int> array, int main_column, int sub_column) {
    float color_for_element = 360.0 / screen_height;
    float angel_per_element = 360.0 / array.size() * pi / 180.0;


    float radius_for_circle = screen_height / 2.2;

    int Ox = screen_width / 2;
    int Oy = screen_height / 2;

    float max_koef = radius_for_circle / screen_height;

    for (int i = 0; i < array.size(); i++) {
        float radius = max_koef * array[i];

        float x1 = radius * cos((i) * angel_per_element);
        float y1 = radius * sin((i) * angel_per_element);

        float xextra = radius * 1.05 * cos((i+0.5) * angel_per_element);
        float yextra = radius * 1.05 * sin((i+0.5) * angel_per_element);

        float x2 = radius * cos((i+1)*angel_per_element);
        float y2 = radius * sin((i+1)*angel_per_element);

        sf::Color color = HSL_to_RGB(color_for_element * (array[i] - 1), 1.0, 0.5);

        window.draw(get_convex_shape( std::vector<float>{ float(Ox), float(Oy), float(Ox + x1), float(Oy + y1), float(Oy + xextra), float(Oy + yextra), float(Ox + x2), float(Oy + y2) }, color, color, 0));

        if (i == main_column || i == sub_column) {
            float x = ((radius / 9) + radius) * cos((i)*angel_per_element);
            float y = ((radius / 9) + radius) * sin((i)*angel_per_element);
            window.draw(get_circle(screen_height / array.size() / 2, Ox + x, Oy + y, sf::Color::White));
        }
    }
}

void ArrayDrawer::draw_pyramid(sf::RenderTarget& window, std::vector<int> array, int main_column, int sub_column) {

    float height_of_pyramid = screen_height / 2.0;
    float Ox = screen_width / 2.0;
    float Oy = screen_height / 2.0 + screen_height / 3.0;

    float color_for_element = 360.0 / screen_height;
    float max_koef = height_of_pyramid / array.size();

    for (int i = array.size() - 1; i >= 0; i--) {
        float x = Ox;
        float y = Oy - (height_of_pyramid - max_koef * i) - height_of_pyramid / 3.0;

        float sizex = array[i] * max_koef;
        float sizey = sizex * max_koef / 2;
        float width = max_koef * color_for_element + 1;

        sf::Color color = HSL_to_RGB(color_for_element * (array[i] - 1), 1.0, 0.5);
        if (i == main_column || i == sub_column) {
            window.draw(get_ellipse(x, y, sizex, sizey, width, column_color));
        }
        else {
            window.draw(get_ellipse(x, y, sizex, sizey, width, color));
        }
    }
}

void ArrayDrawer::draw_array(sf::RenderTarget& window, std::vector<int> array, int main_column, int sub_column, bool reset_screen) {

    if (reset_screen) {
        window.clear(background_color);
    }

    switch (choice) {
    case 1:
        draw_colums(window, array, main_column, sub_column);
        break;
    case 2:
        draw_points(window, array, main_column, sub_column);
        break;
    case 3:
        draw_circumference(window, array, main_column, sub_column);
        break;
    case 4:
        draw_spiral(window, array, main_column, sub_column);
        break;
    case 5:
        draw_pyramid(window, array, main_column, sub_column);
        break;
    default:
        exit(-1);
    }

  
   window.draw(get_text(sort_name, main_text_size, screen_width / 2, screen_height * 0.85, screen_width, screen_height, column_color, background_color, font));
    
    std::string array_lenght_string = "Array lenght: " + std::to_string(array.size());
    window.draw(get_text(array_lenght_string, sub_text_size, screen_width / 2, screen_height * 0.89, screen_width, screen_height, column_color, background_color, font));
    if (max_speed > 0) {
        std::string speed_string = "Speed: " + std::to_string(speed) + "/" + std::to_string(max_speed);

        window.draw(get_text(speed_string, sub_text_size, screen_width / 2, screen_height * 0.82, screen_width, screen_height, column_color, background_color, font));
    }
    if (reset_screen) {
        // The target is the window, or an offscreen texture in record mode.
        if (auto* w = dynamic_cast<sf::RenderWindow*>(&window)) w->display();
        else if (auto* t = dynamic_cast<sf::RenderTexture*>(&window)) t->display();
    }
}

void ArrayDrawer::set_sort_name(std::string sort_name) {
    this->sort_name = sort_name;
}
void ArrayDrawer::set_choice(int choice) {
    this->choice = choice;
}
void ArrayDrawer::set_speed(int speed) {
    this->speed = speed;
}
void ArrayDrawer::set_colors(sf::Color column_color, sf::Color background_color, sf::Color main_column_color, sf::Color sub_column_color) {
    this->column_color = column_color;
    this->background_color = background_color;
    this->main_column_color = main_column_color;
    this->sub_column_color = sub_column_color;
}
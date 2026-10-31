#include "Menu.hpp"
#include <iostream>

Menu::Menu(int screen_width, int screen_height) {
    this->screen_width = screen_width;
    this->screen_height = screen_height;

    vector_lenght = screen_width / 2;
    vector_one_element_size = screen_width / vector_lenght ;

    main_text_size = screen_height / 10;
    sub_text_size = screen_height / 25;

    all_menu_text_size = (sub_text_size + main_text_size / 2) * (menu_text.size() - 0.5);

    start_of_text_in_menu = screen_height - all_menu_text_size;
    start_of_text_in_settings = start_of_text_in_menu / 2;
    
    array_filler();
}

std::string Menu::check_event(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {

        case sf::Event::Closed:
            window.close();
            exit(1);

            break;

        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Enter) { return "enter"; }

            if (event.key.code == sf::Keyboard::Escape) { return "escape"; }

            if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) { return "up"; }
            else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) { return "down"; }

            if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) { return "left"; }
            else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) { return "right"; }

            break;

        case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) { return "up"; }
            else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) { return "down"; }

            break;

        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) { return "enter"; }
            break;

        default:
            break;
        }
    }
    return "";
}

void Menu::array_filler() {
    array = {};
    switch (how_fill_array_choice) {
    case 0:
        fill_vector_in_ascending_order(&array, vector_lenght, vector_one_element_size);
        break;
    case 1:
        fill_vector_in_descending_order(&array, vector_lenght, vector_one_element_size);
        break;
    case 2:
        fill_vector_with_random_nums(&array, vector_lenght, vector_one_element_size, screen_height);
        break;
    }
}

void Menu::how_fill_array(sf::RenderWindow& window, ArrayDrawer& drawer) {
    float Ox = screen_width / 2;
    float Oy = start_of_text_in_settings + (3 * main_text_size);

    float x1 = sub_text_size * 5.5;
    float x2 = sub_text_size * 4.5;

    float y1 = Oy + main_text_size / 5;
    float y2 = Oy - main_text_size / 5;

    window.draw(get_convex_shape(std::vector<float>{Ox - x1, Oy, Ox - x2, y1, Ox - x2, y2}, selected_color, background_color, sub_text_size / 4));
    window.draw(get_convex_shape(std::vector<float>{Ox + x1, Oy, Ox + x2, y1, Ox + x2, y2}, selected_color, background_color, sub_text_size / 4));

    float textOy =  start_of_text_in_settings + main_text_size * 3.5;

    for (int i = 0; i < ways_of_filling_array.size(); i++) {
        if (i == how_fill_array_choice) {
            window.draw(get_text(ways_of_filling_array[i], sub_text_size / 2 * 1.2, Ox + sub_text_size * 6 * (i - 1), textOy, screen_width, screen_height, selected_color, background_color, font));
        }
        else {
            window.draw(get_text(ways_of_filling_array[i], sub_text_size / 2,       Ox + sub_text_size * 6 * (i-1),   textOy, screen_width, screen_height, simple_color,   background_color, font));
        }
    }
}

void Menu::sort_algorithms_order(sf::RenderWindow& window, ArrayDrawer& drawer) {
    window.draw(get_rectangle(0, 0, screen_width, screen_height, sf::Color(255, 255, 255, 100)));

    int table_width = screen_width * 0.8;
    int table_height = screen_height * 0.8;

    int space_from_border_x = (screen_width - table_width) / 2;
    int space_from_border_y = (screen_height - table_height) / 2 * 1.5;

    int move_on_x_for_element = table_width / table_elements_in_row;
    int move_on_y_for_element = table_height / table_elements_in_row;
    int border = 10;

    int text_size = move_on_x_for_element / find_max_len(sorts_table) * 1.5;

    window.draw(get_rectangle(space_from_border_x - border, space_from_border_y - border, table_width + border, table_height + border, background_color));
    window.draw(get_text("Sort order", main_text_size / 2, screen_width / 2, space_from_border_y / 2, screen_width, screen_height, simple_color, background_color, font));
    
    if (sort_choice == -2) {
        window.draw(get_text("Basic order", sub_text_size * 1.2, screen_width * 0.25, space_from_border_y / 2, screen_width, screen_height, selected_color, background_color, font));
        window.draw(get_text("Exit", sub_text_size, screen_width * 0.75, space_from_border_y / 2, screen_width, screen_height, simple_color, background_color, font));
    }
    else if (sort_choice == -1) {
        window.draw(get_text("Basic order", sub_text_size, screen_width * 0.25, space_from_border_y / 2, screen_width, screen_height, simple_color, background_color, font));
        window.draw(get_text("Exit", sub_text_size * 1.2, screen_width * 0.75, space_from_border_y / 2, screen_width, screen_height, selected_color, background_color, font));
    }
    else {
        window.draw(get_text("Basic order", sub_text_size, screen_width * 0.25, space_from_border_y / 2, screen_width, screen_height, simple_color, background_color, font));
        window.draw(get_text("Exit", sub_text_size, screen_width * 0.75, space_from_border_y / 2, screen_width, screen_height, simple_color, background_color, font));
    }

    for (int i = 0; i < table_elements_in_row; i++) {
        for (int j = 0; j < table_elements_in_row; j++) {
            int element_in_table = table_elements_in_row * i + j;

            int x = move_on_x_for_element * j + space_from_border_x;
            int y = move_on_y_for_element * i + space_from_border_y;
            if (element_in_table != sort_choice && element_in_table != remember_sort_choice) {
                window.draw(get_rectangle(x, y, move_on_x_for_element - border, move_on_y_for_element - border, simple_color));
                window.draw(get_rectangle(x, y, move_on_x_for_element - border, move_on_y_for_element - border, sf::Color(255, 255, 255, 200)));
                window.draw(get_text(sorts_table[element_in_table], text_size, x + (move_on_x_for_element - border) / 2, y + (move_on_x_for_element - border) / 2, screen_width, screen_height, simple_color, background_color, font));
            }
            else {
                window.draw(get_rectangle(x, y, move_on_x_for_element - border, move_on_y_for_element - border, selected_color));
                window.draw(get_rectangle(x, y, move_on_x_for_element - border, move_on_y_for_element - border, sf::Color(255, 255, 255, 200)));
                window.draw(get_text(sorts_table[element_in_table], text_size * 1.2, x + (move_on_x_for_element - border) / 2, y + (move_on_x_for_element - border) / 2, screen_width, screen_height, selected_color, background_color, font));

            }
        }
    }
}

void Menu::change_theme(sf::RenderWindow& window, ArrayDrawer& drawer) {

    float Ox = screen_width / 2;
    float Oy = start_of_text_in_settings + (1.5 * main_text_size);

    float x1 = sub_text_size * 4;
    float x2 = sub_text_size * 3;

    float y1 = Oy + main_text_size / 5;
    float y2 = Oy - main_text_size / 5;

    window.draw(get_convex_shape(std::vector<float>{Ox - x1, Oy, Ox - x2, y1, Ox - x2, y2}, selected_color, background_color, sub_text_size/4));
    window.draw(get_convex_shape(std::vector<float>{Ox + x1, Oy, Ox + x2, y1, Ox + x2, y2}, selected_color, background_color, sub_text_size/4));
}

void Menu::settings_menu(sf::RenderWindow& window, ArrayDrawer& drawer) {
    window.draw(get_rectangle(0, 0, screen_width, screen_height, sf::Color(255, 255, 255, 100)));
    window.draw(get_text(settings_text[0], main_text_size, screen_width / 2, start_of_text_in_settings, screen_width, screen_height, simple_color, background_color, font));

    for (int i = 1; i < settings_text.size(); i++) {
        if (i == settings_choice) {
            window.draw(get_text(settings_text[i], sub_text_size * 1.2, screen_width / 2, start_of_text_in_settings + (i) * main_text_size * 1.5, screen_width, screen_height, selected_color, background_color, font));
        }
        else {
            window.draw(get_text(settings_text[i], sub_text_size, screen_width / 2, start_of_text_in_settings + (i) * main_text_size * 1.5, screen_width, screen_height, simple_color, background_color, font));
        }
    }
}

void Menu::main_menu(sf::RenderWindow& window, ArrayDrawer& drawer) {
    window.draw(get_circle(circle_size, screen_width / 2 - circle_size, screen_height / 2 - circle_size, sf::Color(255, 255, 255, 100)));
    window.draw(get_text(menu_text[0], main_text_size, screen_width / 2, start_of_text_in_menu, screen_width, screen_height, simple_color, background_color, font));

    for (int i = 1; i < menu_text.size(); i++) {
        if (i == main_menu_choice) {
            window.draw(get_text(menu_text[i], sub_text_size * 1.2, screen_width / 2, start_of_text_in_menu + (i + 1) * main_text_size / 2, screen_width, screen_height, selected_color, background_color, font));
        }
        else {
            window.draw(get_text(menu_text[i], sub_text_size, screen_width / 2, start_of_text_in_menu + (i + 1) * main_text_size / 2, screen_width, screen_height, simple_color, background_color, font));
        }
    }
}

int Menu::start(sf::RenderWindow& window) {
    ArrayDrawer drawer(screen_width, screen_height, vector_one_element_size, 0);

    simple_color = themes[themes_choice][0], selected_color = themes[themes_choice][1], background_color = themes[themes_choice][2];
    drawer.set_colors(simple_color, background_color, simple_color, simple_color);

    while (window.isOpen()) {

        window.clear(background_color);
        drawer.draw_array(window, array, -1, -1, false);
        
        std::string event_check;

        switch (menu_right_now_choice) {

        case 0:
            event_check = check_event(window);
            

            if (event_check == "escape") { exit(0); }
            else if (event_check == "enter") {
                if (menu_text[main_menu_choice] == "Settings") { menu_right_now_choice = 1; }
                else if (menu_text[main_menu_choice] == "Exit") { exit(0); }
                else { return main_menu_choice; }
            }
            else if (event_check == "up") { main_menu_choice--; }
            else if (event_check == "down") { main_menu_choice++; }

            check_if_number_is_less_or_greater(main_menu_choice, 1, menu_text.size() - 1);
            if (main_menu_choice < menu_text.size() - 2) { drawer.set_choice(main_menu_choice); }

            main_menu(window, drawer);
            break;

        case 1:
            event_check = check_event(window);
            check_if_number_is_less_or_greater(settings_choice, 1, settings_text.size() - 1);

            if (event_check == "escape") { menu_right_now_choice = 0; }
            else if (event_check == "enter") {
                if (settings_text[settings_choice] == "Themes") { menu_right_now_choice = 2; }
                else if (settings_text[settings_choice] == "How Fill Array") { menu_right_now_choice = 3; }
                else if (settings_text[settings_choice] == "Sort algorithms order") { menu_right_now_choice = 4; }
                else if (settings_text[settings_choice] == "Exit") { menu_right_now_choice = 0; }
                else { return main_menu_choice; }
            }
            else if (event_check == "up") { settings_choice--; }
            else if (event_check == "down") { settings_choice++; }

            settings_menu(window, drawer);
            break;

        case 2:
            event_check = check_event(window);

            if (event_check ==  "escape" || event_check == "enter") { menu_right_now_choice = 1; }
            else if (event_check == "left") { themes_choice--; }
            else if (event_check == "right") { themes_choice++; }

            check_if_number_is_less_or_greater(themes_choice, 0, themes.size() - 1);

            simple_color = themes[themes_choice][0], selected_color = themes[themes_choice][1], background_color = themes[themes_choice][2];
            drawer.set_colors(simple_color, background_color, simple_color, simple_color);
            settings_menu(window, drawer);
            change_theme(window, drawer);
            break;

        case 3:
            event_check = check_event(window);
            
            if (event_check == "escape" || event_check == "enter") { menu_right_now_choice = 1; }
            else if (event_check == "left") { how_fill_array_choice--; }
            else if (event_check == "right") { how_fill_array_choice++; }

            check_if_number_is_less_or_greater(how_fill_array_choice, 0, ways_of_filling_array.size() - 1);
            array_filler();

            settings_menu(window, drawer);
            how_fill_array(window, drawer);
            break;
        case 4:
            event_check = check_event(window);

            if (event_check == "escape") { menu_right_now_choice = 1; remember_sort_choice = -1;}
            else if (event_check == "enter") {
                if (sort_choice == -1) { menu_right_now_choice = 1; remember_sort_choice = -1; }
                else if (sort_choice == -2) { sorts_table = get_sorts_table_origin(); sort_choice = 0; remember_sort_choice = -1; }

                if (remember_sort_choice == -1) {
                    remember_sort_choice = sort_choice;
                }
                else {
                    swap(sorts_table[remember_sort_choice], sorts_table[sort_choice]);
                    remember_sort_choice = -1;
                }
            }
            else if (event_check == "left") { 
                if (sort_choice == -1) { sort_choice = -2; }
                else if (sort_choice == -2) { sort_choice = -1; }
                else {
                    sort_choice--;
                    check_if_number_is_less_or_greater(sort_choice, 0, sorts_table.size() - 1);
                }
            }
            else if (event_check == "right") { 
                if (sort_choice == -1) { sort_choice = -2; }
                else if (sort_choice == -2) { sort_choice = -1; }
                else {
                    sort_choice++;
                    check_if_number_is_less_or_greater(sort_choice, 0, sorts_table.size() - 1);
                }
            }
            else if (event_check == "up") { 
                if (!(sort_choice == -1 || sort_choice == -2)) {
                    if (sort_choice == 0 || sort_choice == 1) {
                        sort_choice = -2;
                    }
                    else if (sort_choice == 2 || sort_choice == 3) {
                        sort_choice = -1;
                    }
                    else {
                        sort_choice -= table_elements_in_row;
                        check_if_number_is_less_or_greater(sort_choice, 0, sorts_table.size() - 1);
                    }
                }
            }
            else if (event_check == "down") { 
                if (sort_choice == -1 || sort_choice == -2) { sort_choice = 0; }
                else {
                    sort_choice += table_elements_in_row;
                    check_if_number_is_less_or_greater(sort_choice, 0, sorts_table.size() - 1);
                }
            }

            sort_algorithms_order(window, drawer);
            break;
        default:
            break;
        }

        window.display();
    }
}

std::vector<sf::Color> Menu::get_colors() {
    return themes[themes_choice];
}
void Menu::set_theme(int theme) {
    themes_choice = theme;
    check_if_number_is_less_or_greater(themes_choice, 0, themes.size() - 1);
}
std::vector<int> Menu::get_array() {
    return array;
}
std::vector<std::string> Menu::get_sort_table() {
    return sorts_table;
}

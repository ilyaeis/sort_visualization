#include "AnotherFunc.hpp"

// --- get sfml shapes
sf::RectangleShape get_rectangle(int x, int y, int width, int height, sf::Color color) {
    sf::RectangleShape rectangle(sf::Vector2f(width, height));

    rectangle.setPosition(x, y);
    rectangle.setFillColor(color);

    return rectangle;
}
sf::ConvexShape get_convex_shape(std::vector<float> cords, sf::Color color, sf::Color outline_color, int outline_size) {
    sf::ConvexShape convex_shape(cords.size() / 2);

    for (int i = 0; i < cords.size() / 2; i++) {
        convex_shape.setPoint(i, sf::Vector2f(cords[i * 2], cords[i * 2 + 1]));
    }
    convex_shape.setFillColor(color);

    convex_shape.setOutlineThickness(outline_size);
    convex_shape.setOutlineColor(outline_color);

    return convex_shape;
}

sf::Vertex* get_line(float x1, float y1, float x2, float y2, sf::Color color) {
    sf::Vertex line[2];
    line[0].position = sf::Vector2f(x1, y1);
    line[0].color = color;
    line[1].position = sf::Vector2f(x2, y2);
    line[1].color = color;

    return line;
}
sf::CircleShape get_circle(float radius, float x, float y, sf::Color color) {

    sf::CircleShape circle(radius);
    circle.setFillColor(color);
    circle.setPosition(x, y);

    return circle;
}

sf::CircleShape get_ellipse(float x, float y, float sizex, float sizey, float width, sf::Color color) {
    sf::CircleShape ellipse(1);

    ellipse.setPosition(x - sizex / 4, y - sizey / 8);
    ellipse.setOrigin(0, 0);

    ellipse.setScale(sf::Vector2f(sizex / 4, sizey / 8));

    ellipse.setFillColor(sf::Color::Transparent);
    ellipse.setOutlineThickness(width / (sizey / 8));
    ellipse.setOutlineColor(color);

    return ellipse;
}

// here x, y is distance from center
sf::Text get_text(std::string text_line, int size, int x, int y, int screen_width, int screen_height, sf::Color color, sf::Color outline_color, sf::Font& font) {
    sf::Text  text;

    text.setCharacterSize(size);
    text.setString(text_line);
    text.setFillColor(color);
    text.setFont(font);

    text.setOutlineThickness(size / 10);
    text.setOutlineColor(outline_color);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));

    return text;
}

// The font bytes are compiled in (see CMakeLists.txt), so no file lookup is needed.
extern const unsigned char font_data[];
extern const unsigned long font_data_size;

sf::Font get_font() {
    sf::Font font = sf::Font{};
    if (!font.loadFromMemory(font_data, font_data_size))
    {
        sf::err() << "Couldn't load font\n";
    }
    return font;
}

// --- color converter
sf::Color HSL_to_RGB(float h, float s, float l) {

    float C = (1 - abs(2 * l - 1)) * s;
    float X = C * (1 - abs(fmodf(h / 60, 2) - 1));
    float m = l - C / 2;

    if (h >= 0 && h < 60) { return sf::Color((C + m) * 255, (X + m) * 255, (0 + m) * 255); }
    else if (h >= 60 && h < 120) { return sf::Color((X + m) * 255, (C + m) * 255, (0 + m) * 255); }
    else if (h >= 120 && h < 180) { return sf::Color((0 + m) * 255, (C + m) * 255, (X + m) * 255); }
    else if (h >= 180 && h < 240) { return sf::Color((0 + m) * 255, (X + m) * 255, (C + m) * 255); }
    else if (h >= 240 && h < 300) { return sf::Color((X + m) * 255, (0 + m) * 255, (C + m) * 255); }
    else if (h >= 300 && h < 360) { return sf::Color((C + m) * 255, (0 + m) * 255, (X + m) * 255); }
}

// --- find index of nums
int find_index_of_max(std::vector<int> array) {
    int remember = 0;
    for (int i = 0; i < array.size(); i++) {
        if (array[i] > array[remember]) {
            remember = i;
        }
    }
    return remember;
}

int index_of_num_next_after(int num, std::vector<int> array) {
    int remember = find_index_of_max(array);
    for (int i = 0; i < array.size(); i++) {
        if (array[i] > num && array[i] <= array[remember]) {
            remember = i;
        }
    }
    return remember;
}

void check_if_number_is_less_or_greater(int &num, int smallest, int biggest) {
    if (num > biggest) {
        num = smallest;
    }
    else if (num < smallest) {
        num = biggest;
    }

}
// --- find lenght of longest string in array
int find_max_len(std::vector<std::string> array) {
    int max_len = 0;
    for (std::string i : array) {
        if (i.size() > max_len) { max_len = i.size(); }
    }
    return max_len;
}
// work with vectors
// --- fill vector
void fill_vector_in_ascending_order(std::vector<int>* array, int lenght, int vector_one_item_size) {
    for (int i = 0; i < lenght; i++) {
        (*array).push_back((i + 1) * vector_one_item_size);
    }
}

// dont work with circumference vizualization
void fill_vector_with_random_nums(std::vector<int>* array, int lenght, int vector_one_item_size, int max_num) {
    for (int i = 0; i < lenght; i++) {
        (*array).push_back((rand() % max_num) + 1);
    }
}

void fill_vector_in_descending_order(std::vector<int>* array, int lenght, int vector_one_item_size) {
    for (int i = 0; i < lenght; i++) {
        (*array).push_back((i + 1) * vector_one_item_size);
    }
    for (int i = 0; i < lenght / 2; i++) {
        swap(&(*array)[i], &(*array)[lenght - i - 1]);
    }
}

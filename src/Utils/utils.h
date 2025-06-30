#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <vector>

class menu_template
{
    public:
    menu_template();
    menu_template(sf::Font font);
    void add_elements(std::vector<std::string> element_text);
    void add_elements(std::vector<std::string> element_text, int index);
    void remove_element(int index);
    void generate_drawables(unsigned int window_size[2], unsigned int offset, unsigned int char_size, sf::Vector2f mouse_pos);
    std::tuple<std::vector<sf::RectangleShape>, std::vector<std::string>> get_drawables();
    private:
    const sf::Font font {"C:/Windows/Fonts/arial.ttf"};
    // unsigned int char_size;
    unsigned int window_size_class[2] {0, 0};
    std::vector<sf::RectangleShape> element_rects;
    std::vector<sf::Text> element_texts;
    int elements_length {0};
    unsigned int offset_class[2] {0,0};
};

#endif
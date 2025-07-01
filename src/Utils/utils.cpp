#include <stdio.h>
#include <iostream>
#include <vector>
#include "utils.h"

menu_template::menu_template()
{
    
}

menu_template::menu_template(sf::Font font) : font {font}
{

}

void menu_template::add_elements(std::string element_text[], int curr_elements_length)
{
    std::cout << "curr_elements_length" << curr_elements_length << std::endl;
    for (int i = 0; i < curr_elements_length; i++) {
        //texts
        sf::Text sf_text(font);
        sf_text.setString(element_text[i]);
        sf_text.setOrigin(sf_text.getLocalBounds().getCenter());
        element_texts.push_back(sf_text);
        //rectangles
        sf::RectangleShape element_rect;
        element_rect.setFillColor(sf::Color::Transparent);
        element_rect.setOutlineThickness(10.f);
        element_rect.setOutlineColor(sf::Color::White);
        element_rect.setOrigin(element_rect.getLocalBounds().getCenter());
        element_rects.push_back(element_rect);
        //nr element_rects
    }
    elements_length+=curr_elements_length;
    for (int i=0; i<elements_length; i++) {
        
    }
}

void menu_template::add_elements(std::string element_text[], int curr_elements_length, int index)
{
// missing
}

void menu_template::remove_element(int index)
{
//missing
}

void menu_template::generate_drawables(unsigned int window_size[2], unsigned int offset[2], unsigned int char_size, sf::Vector2f mouse_pos)
{
    if (window_size!=window_size_class){
        float fraction_of_window {0.4};
        unsigned int space_between {50};
        float center_y=window_size[1]/2;
        float start_x=window_size[0]*fraction_of_window/2;
        for (size_t i=0; i<elements_length; i++){
            float center_x=start_x+((window_size[0]*fraction_of_window)/elements_length)*i;
            element_rects[i].setPosition({center_y, center_x});
            element_rects[i].setSize({100.f, 20.f});
            element_texts[i].setPosition({center_y, center_x});
        }
        window_size_class[0]=window_size[0];
        window_size_class[1]=window_size[1];
    }
}

std::tuple<std::vector<sf::RectangleShape>, std::vector<sf::Text>> menu_template::get_drawables()
{
    return std::make_tuple(element_rects, element_texts);
}
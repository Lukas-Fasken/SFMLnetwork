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

void menu_template::add_elements(std::vector<std::string> element_text)
{
    for (const auto& text : element_text) {
        //texts
        sf::Text sf_text(font);
        sf_text.setString(text);
        sf_text.setOrigin(sf_text.getLocalBounds().getCenter());
        element_texts.push_back(sf_text);
        //rectangles
        sf::RectangleShape element_rect;
        element_rect.setFillColor(sf::Color::Transparent);
        element_rect.setOrigin(element_rect.getLocalBounds().getCenter());
        element_rects.push_back(element_rect);
        //nr element_rects
        elements_length+=1;
    }
    for (int i=0; i<elements_length; i++) {
        
    }
}

void menu_template::add_elements(std::vector<std::string> element_text, int index)
{
// missing
}

void menu_template::generate_drawables(unsigned int window_size[2], unsigned int offset, unsigned int char_size, sf::Vector2f mouse_pos)
{
    if (window_size!=window_size_class){
        for (size_t i=0; i<elements_length; i++){
            
        }
        window_size_class[0]=window_size[0];
        window_size_class[1]=window_size[1];
    }
}
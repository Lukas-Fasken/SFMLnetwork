#include <SFML/Graphics.hpp>
#include "Sockets/socket.h"
#include "Utils/utils.h"

int main()
{
    unsigned int window_size[] {1000u, 1000u};
    auto window = sf::RenderWindow(sf::VideoMode({window_size[0], window_size[1]}), "SFMLNetwork");
    window.setFramerateLimit(144);

    //socket setup
    Socket socket;
    socket.Create_socket(SOCK_STREAM, 0);
    socket.connect_socket("100.113.234.57");

    //text setup
    sf::Font font("C:/Windows/Fonts/arial.ttf");
    std::string printed_string;
    sf::Text printed_text(font);

    //menu setup
    menu_template menu(font);
    std::string element_text[]={"one", "two", "three", "four", "five"};
    int curr_elements_length = sizeof(element_text) / sizeof(element_text[0]);
    menu.add_elements(element_text, curr_elements_length);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        unsigned int offset[]={0,0};
        unsigned int char_size =10;
        sf::Vector2f mouse_pos = {0,0};
        menu.generate_drawables(window_size, offset, char_size, mouse_pos);

        std::tuple<std::vector<sf::RectangleShape>, std::vector<sf::Text>> menu_drawables=menu.get_drawables();
        window.clear();
        for (auto rect : std::get<0>(menu_drawables)) {
            window.draw(rect);
        }
        for (auto text : std::get<1>(menu_drawables)) {
            window.draw(text);
        }
        window.display();
    }
    socket.close_socket();
}

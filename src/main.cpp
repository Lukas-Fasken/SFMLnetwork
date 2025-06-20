#include <SFML/Graphics.hpp>
#include "Sockets/socket.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "SFMLNetwork");
    window.setFramerateLimit(144);

    Socket socket;
    socket.Create_socket(SOCK_STREAM, 0);
    socket.connect_socket("100.113.234.57");

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
    socket.close_socket();
}

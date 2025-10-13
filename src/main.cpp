#include <SFML/Network.hpp>
#include <iostream>

int main() {
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect({192, 168, 0, 5}, 53000);
    std::cout << "Connected to server!\n";

    while (true) {
        std::string msg;
        std::getline(std::cin, msg);

        sf::Packet packet;
        packet << msg;
        socket.send(packet);

        sf::Packet recvPacket;
        if (socket.receive(recvPacket) == sf::Socket::Status::Done) {
            std::string reply;
            recvPacket >> reply;
            std::cout << "Server says: " << reply << "\n";
        }
    }
}

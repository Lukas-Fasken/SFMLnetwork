#include <SFML/Network.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <atomic>
#include <csignal>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>
using json = nlohmann::json;

// Global running flag
std::atomic<bool> g_running{true};

// Outgoing message queue and synchronization primitives
std::mutex outMutex;
std::condition_variable outCv;
std::vector<std::string> outQueue;


void signal_handler(int) {
    g_running.store(false);
}

void input_thread_func()
{
    std::string line;
    while (g_running.load()) {
        if (!std::getline(std::cin, line)) break;
        if (!g_running.load()) break;

        if (line == "/quit") {
            g_running.store(false);
            break;
        } else {
            // Accept two styles:
            // 1) /json <type> <filepath>  -> read file and send JSON {type: <type>, data: <parsed JSON or raw string>}
            // 2) /json <type> <string>    -> send JSON {type: <type>, data: <string>}
            // 3) <type> <string>          -> shorthand without /json prefix

            // tokenize: first token may be "/json"
            std::istringstream iss(line);
            std::string first;
            if (!(iss >> first)) continue;

            std::string type;
            std::string rest;

            if (first == "/json") {
                if (!(iss >> type)) {
                    std::cerr << "Usage: /json <type> <filepath|string>\n";
                    continue;
                }
                std::getline(iss, rest);
                if (!rest.empty() && rest.front() == ' ') rest.erase(0,1);
            } else {
                // treat first token as type and the rest as string
                type = first;
                std::getline(iss, rest);
                if (!rest.empty() && rest.front() == ' ') rest.erase(0,1);
            }

            if (type.empty()) {
                std::cerr << "Message type is required.\n";
                continue;
            }

            json outj;
            outj["type"] = type;

            if (rest.empty()) {
                // empty data
                outj["data"] = nullptr;
            } else {
                // if rest looks like an existing file path, read it
                try {
                    if (std::filesystem::exists(rest)) {
                        std::ifstream ifs(rest);
                        if (!ifs) {
                            std::cerr << "Failed to open file: " << rest << "\n";
                            continue;
                        }
                        std::string contents((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
                        try {
                            // If file contains valid JSON, use parsed value as data
                            json parsed = json::parse(contents);
                            outj["data"] = parsed;
                        } catch (...) {
                            // otherwise send raw contents as string
                            outj["data"] = contents;
                        }
                    } else {
                        // not a file, treat rest as a raw string
                        outj["data"] = rest;
                    }
                } catch (const std::exception &e) {
                    std::cerr << "Error processing data or file: " << e.what() << "\n";
                    continue;
                }
            }

            // queue serialized JSON
            try {
                std::string serialized = outj.dump();
                {
                    std::lock_guard<std::mutex> lock(outMutex);
                    outQueue.push_back(serialized);
                }
                outCv.notify_one();
            } catch (const std::exception &e) {
                std::cerr << "Failed to serialize JSON message: " << e.what() << "\n";
            }
        }
    }
}

static const char* socketStatusToString(sf::Socket::Status status)
{
    switch (status)
    {
        case sf::Socket::Status::Done: return "Done";
        case sf::Socket::Status::NotReady: return "NotReady";
        case sf::Socket::Status::Partial: return "Partial";
        case sf::Socket::Status::Disconnected: return "Disconnected";
        case sf::Socket::Status::Error: return "Error";
        default: return "Unknown";
    }
}

int main()
{
    // Basic SIGINT handler for graceful shutdown (Ctrl+C)
    std::signal(SIGINT, signal_handler);

    const std::string host = "127.0.0.1"; // default to localhost; change as needed
    const unsigned short serverPort = 54001;

    auto resolved = sf::IpAddress::resolve(host);
    if (!resolved) {
        std::cerr << "Failed to resolve host: " << host << "\n";
        return 1;
    }
    sf::IpAddress serverAddress = *resolved;

    sf::TcpSocket socket;

    std::cout << "Connecting to " << serverAddress.toString() << ":" << serverPort << "...\n";
    if (socket.connect(serverAddress, serverPort, sf::seconds(5.f)) != sf::Socket::Status::Done) {
        std::cerr << "Failed to connect to server.\n";
        return 1;
    }

    std::cout << "Connected to server. Type messages to send. Type /quit to exit.\n";

    // Start input thread
    std::thread input_thread(input_thread_func);

    sf::SocketSelector selector;
    selector.add(socket);

    // Send an initial handshake as JSON
    {
        json h;
        h["type"] = "hello";
        h["name"] = "client";
        sf::Packet p;
        p << h.dump();
        if (socket.send(p) != sf::Socket::Status::Done) {
            std::cerr << "Failed to send handshake.\n";
        }
    }

    // After successful connect and handshake, switch to non-blocking for main loop
    socket.setBlocking(false);

    while (g_running.load()) {
        // Wait for socket activity or timeout
        if (selector.wait(sf::milliseconds(500))) {
            if (selector.isReady(socket)) {
                // Receive one packet
                sf::Packet in;
                sf::Socket::Status st = socket.receive(in);
                if (st == sf::Socket::Status::Done) {
                    std::string msg;
                    if (in >> msg) {
                        try {
                            json j = json::parse(msg);
                            std::cout << "[Server] " << j.dump() << "\n";
                        } catch (...) {
                            std::cout << "[Server] " << msg << "\n";
                        }
                    }
                } else if (st == sf::Socket::Status::Disconnected) {
                    std::cout << "Disconnected by server.\n";
                    g_running.store(false);
                    break;
                } else if (st == sf::Socket::Status::NotReady) {
                    // nothing
                } else {
                    std::cerr << "Socket error: " << socketStatusToString(st) << "\n";
                }
            }
        }

        // Drain outgoing queue (console input)
        std::vector<std::string> localOut;
        {
            std::lock_guard<std::mutex> lock(outMutex);
            if (!outQueue.empty()) localOut.swap(outQueue);
        }
        for (auto &line : localOut) {
            // Try to parse as JSON, otherwise wrap
            json outj;
            try {
                outj = json::parse(line);
            } catch (...) {
                outj["type"] = "message";
                outj["data"] = line;
            }
            sf::Packet p;
            p << outj.dump();
            sf::Socket::Status sendStatus = socket.send(p);
            if (sendStatus != sf::Socket::Status::Done) {
                std::cerr << "Failed to send message (status=" << socketStatusToString(sendStatus) << ")\n";
            }
        }
    }

    std::cout << "Shutting down client...\n";
    socket.disconnect();
    if (input_thread.joinable()) input_thread.join();
    return 0;
}

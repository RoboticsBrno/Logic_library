#include "Logic.hpp"
#include <iostream>
#include <map>
#include <sstream>

#include "rbtcp.h"
#include "rbwifi.h"

using namespace rb;

// Vyberte port z rozmezí 15000 až 15100. Všechny Logiky na stejném portu si
// posílají zprávy mezi sebou.
static const uint16_t SERVER_PORT = 15000;

static const std::string SERVER_IP = "192.168.1.112";

static const uint16_t myId = random(0, UINT16_MAX);

static int posX = random(0, 10);
static int posY = random(0, 10);

static std::map<uint16_t, std::pair<int, int>> othersPos;
static std::map<uint16_t, uint32_t> othersTime;

static void sendMyPos(TcpSocket socket) {
    std::ostringstream ss;
    ss << "|" << myId << " " << posX << " " << posY << "\n";
    socket.send(ss.str());
}

void logicMain() {
    printf("Connrect...\n");
    WiFi::connect("DetskyKolotoc", "kouzelnaskolka");

    printf("Cekani na pripojeni na WiFi...\n");
    WiFi::waitForIp();

    auto socket = Tcp::connect(SERVER_IP, SERVER_PORT);

    socket.onReceive([](const std::string& data) {
        uint16_t id;
        int x, y;
        if (sscanf(data.c_str(), "|%hi %i %i\n", &id, &x, &y) > 0) {
            othersPos[id] = std::make_pair(x, y);
            othersTime[id] = millis();
        }
    });

    buttons.onPress([=] {
        posY -= 1;
        if (posY < 0)
            posY = 9;
        sendMyPos(socket);
    },
        Up);

    buttons.onPress([=] {
        posY += 1;
        if (posY > 9)
            posY = 0;
        sendMyPos(socket);
    },
        Down);

    buttons.onPress([=] {
        posX -= 1;
        if (posX < 0)
            posX = 9;
        sendMyPos(socket);
    },
        Left);

    buttons.onPress([=] {
        posX += 1;
        if (posX > 9)
            posX = 0;
        sendMyPos(socket);
    },
        Right);

    int sendTimout = 0;
    while (true) {
        if (sendTimout > 1000) {
            sendMyPos(socket);
            sendTimout = 0;
        }

        const auto curMillis = millis();

        display.clear();
        display.setColor(posX, posY, Green);

        for (auto itr = othersPos.begin(); itr != othersPos.end();) {
            display.setColor(itr->second.first, itr->second.second, Blue);

            auto time = othersTime[itr->first];
            if (curMillis - time > 5000) {
                othersTime.erase(itr->first);
                itr = othersPos.erase(itr);
            } else {
                ++itr;
            }
        }

        display.show(20);

        delay(30);
        sendTimout += 30;
    }
}

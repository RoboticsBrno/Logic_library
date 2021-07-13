// Ukázkový program na připojení RBController aplikace
// Android: https://play.google.com/store/apps/details?id=com.tassadar.rbcontroller
// Windows: https://github.com/RoboticsBrno/rbcontroller-electron/releases

#include "Logic.hpp"
#include <iostream>

// Vložení include souborů z GridUI a RBProtocol knihoven
#include "gridui.h"
#include "rbprotocol.h"
#include "rbwebserver.h"
#include "rbwifi.h"

// Importování tzv. namespace rb a gridui, jinak bychom museli před metody psát rb:: a gridui::
using namespace rb;
using namespace gridui;

// Importování layoutu obrazovky pro RBController
// #define GRIDUI_LAYOUT_DEFINITION musí být jen v jednom .cpp souboru, v ostatních jen #include "layout.hpp" bez #define
#define GRIDUI_LAYOUT_DEFINITION
#include "layout.hpp"

void logicMain() {
    // Připojení na WiFi
    WiFi::connect("DetskyKolotoc", "kouzelnaskolka");
    // Založení Access Pointu - zakomentujte řádek nad tímto s WiFi::connect a odkomentujte ten další s WiFi::startAp
    //WiFi::startAp("MujLogik", "HesloHeslo");

    // Inicializace RBProtokolu
    //
    // !!! ZMĚŇTE TADY JMÉNO FrantaFlinta ZA VAŠE, A STEJNÉ NASTAVTE V RbController APLIKACI !!!
    //
    auto protocol = new Protocol("FrantaFlinta", "Logika", "Compiled at " __DATE__ " " __TIME__, [](const std::string& cmd, rbjson::Object* pkt) {
        UI.handleRbPacket(cmd, pkt);
    });
    protocol->start();
    rb_web_start(80);

    // Inicializace UI knihovny
    UI.begin(protocol);

    // Sestavení prvků UI. Pozice a vlastnosti jsou nastaveny z RBGridUI builderu, takže tady obvykle jen nastavujete
    // zpracování událostí.
    auto builder = Layout.begin();

    // Událost při změně stavu Checkboxu
    builder.Display.onChanged([](Checkbox& cb) {
        if (cb.checked()) {
            display.fill(Rgb(255, 0, 0));
        } else {
            display.clear();
        }
        display.show(Layout.Intensity.value());
    });

    // Při změně hodnoty Slideru
    builder.Intensity.onChanged([](Slider& s) {
        display.show(s.value());
    });

    // Při stisku...
    builder.Buzzer.onPress([](Button&) {
        buzzer.on();
    });

    // ..a uvolnění tlačítka
    builder.Buzzer.onRelease([](Button&) {
        buzzer.off();
    });

    // Zkompletování UI. Za tímto voláním už nejde upravovat nic na builder objektu!
    builder.commit();

    // Stále se ale dá dostat k jednotlivým prvkům, a to přes Layout objekt odkudkoliv z kódu:
    Layout.Buzzer.setColor("green");

    while (true) {
        printf("Intensity slider: %f\n", Layout.Intensity.value());
        waitSeconds(3);
    }
}

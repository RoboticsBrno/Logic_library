// Tato hra je napadobenina hry [T-Rex Dinosaur](https://chromedino.com/) z webového prohlížeče Google Chrome.
// Hra se v prohlížeči Chrome zobrazí při výpadku internetového připojení.

#include "Logic.hpp"
#include <iostream>

void nakresliZem() {
    display.nakresliCaru(0, 9, 9, 9, Zluta);
}

void nakresliDinosauraNaZemi(Rgb barva_dinosaura = Zelena) {
    display.nakresliCaru(1, 8, 1, 5, barva_dinosaura);
}

void nakresliDinosauraVeSkoku(Rgb barva_dinosaura = Zelena) {
    display.nakresliCaru(1, 4, 1, 0, barva_dinosaura);
}

void nakresliPrekazku(int x) {
    display.nastavBarvu(x, 8, Cervena);
}

int pozice_prekazky_x = 9;
void upravPoziciPrekazky() {
    pozice_prekazky_x = pozice_prekazky_x - 1;
    if (pozice_prekazky_x < 0) {
        pozice_prekazky_x = 9;
    }
}

bool dinosaurus_ve_skoku = false;

void logicMain() {
    buttons.priStisku([]() { dinosaurus_ve_skoku = true; }, Up);

    while (true) {
        display.vycisti();
        nakresliZem();
        nakresliPrekazku(pozice_prekazky_x);
        if (dinosaurus_ve_skoku) {
            nakresliDinosauraVeSkoku();
            dinosaurus_ve_skoku = false;
        } else {
            if (pozice_prekazky_x == 1) {
                nakresliDinosauraNaZemi(Cervena);
                display.ukaz(30);
                delay(1500);
                esp_restart();
            }
            nakresliDinosauraNaZemi();
        }
        upravPoziciPrekazky();
        display.ukaz(30);
        delay(1000);
    }
}
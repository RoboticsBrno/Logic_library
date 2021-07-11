#include "Logic.hpp"
#include <bootloader_random.h>

static const int JAS_DISPLEJE = 40;

// Herní dlaždice
struct Dlazdice {
    Dlazdice(Obdelnik obdelnik, Rgb barva, ButtonID tlacitko)
        : obdelnik(obdelnik)
        , barva(barva)
        , tlacitko(tlacitko) {
    }

    Obdelnik obdelnik;
    Rgb barva;
    ButtonID tlacitko;
};

// Obdélníky pro herní draždice
static const std::array<Dlazdice, 5> dlazdice = {
    Dlazdice(Obdelnik(3, 0, 4, 3), Rgb(255, 0, 0), Up),
    Dlazdice(Obdelnik(0, 3, 3, 4), Rgb(0, 255, 0), Left),
    Dlazdice(Obdelnik(3, 3, 4, 4), Rgb(0, 0, 255), Enter),
    Dlazdice(Obdelnik(7, 3, 3, 4), Rgb(0, 255, 255), Right),
    Dlazdice(Obdelnik(3, 7, 4, 3), Rgb(255, 0, 255), Down),
};

// Kolik kroků má sekvence, postupně se zvyšuje
static int gKroky = 3;
// Vzorová sekvence. std::vector == pole, jedno číslo za druhým v paměti
static std::vector<int> gSekvencePredloha;
// Sekvence, kterou zadal uživatel
static std::vector<int> gSekvenceZadana;

// Stav:
// * 0: čekáme na stisknutí tlačítka, abychom vygenerovali sekvenci a začli hru.
// * 1: vyblikáváme vzorovou sekvenci
// * 2: uživatel mačká tlačíka a my je nahráváme do sequence_user
// * 3: zkontrolujeme zadanou sekvenci a vyblikáme výsledek
static std::atomic<int> gStav = 0;

static void priZmeneTlacitka(std::bitset<MaxID> stisknutaTlacitka, std::bitset<MaxID> zmenenaTlacitka) {
    if (gStav == 0) {
        if (stisknutaTlacitka.any()) {
            // Přepneme stav na vyblikávání vzoru
            gStav = 1;
        }
    } else if (gStav == 2) {

        for (int i = 0; i < dlazdice.size(); ++i) {
            const auto& t = dlazdice[i];

            if (stisknutaTlacitka[t.tlacitko]) {
                display.nakresliObdelnikVyplneny(t.obdelnik, t.barva);
            } else if (zmenenaTlacitka[t.tlacitko] && !stisknutaTlacitka[t.tlacitko]) {
                display.nakresliObdelnikVyplneny(t.obdelnik, Rgb(0, 0, 0));

                // Zapíšeme jeho id do uživatelské sekvence
                gSekvenceZadana.push_back(i);

                // Zkontrolujeme, kolik uživatel zadal kroků
                if (gSekvenceZadana.size() >= gKroky) {
                    // Pokud dost, přejdeme do dalšího stavu
                    gStav = 3;
                    break;
                }
            }
        }
        display.ukaz(JAS_DISPLEJE);
    }
}

void logicMain() {
    // Vykreslení základního stavu
    display.nakresliObdelnikVyplneny(0, 0, 10, 10, Rgb(40, 40, 40));
    for (const auto& t : dlazdice) {
        display.nakresliObdelnikVyplneny(t.obdelnik, Rgb(0, 0, 0));
    }
    display.ukaz(JAS_DISPLEJE);

    logic.buttons().priZmene(priZmeneTlacitka);

    // Nekonečná smyčka, ve které se kontroluje, zda se změnil stav
    // do některého z delších úseků.
    while (true) {
        // vyblikáváme vzorovou sekvenci
        if (gStav == 1) {
            // Doplníme sekvenci náhodných čísel 0 až 4
            while (gSekvencePredloha.size() < gKroky) {
                gSekvencePredloha.push_back(nahodne(0, 5));
            }

            // Vyblikáme vzorovou sekvenci
            for (auto idx : gSekvencePredloha) {
                const auto& t = dlazdice[idx];
                cekejVteriny(0.4);
                display.nakresliObdelnikVyplneny(t.obdelnik, t.barva);
                display.show(JAS_DISPLEJE);
                cekejVteriny(0.4);
                display.nakresliObdelnikVyplneny(t.obdelnik, Rgb(0, 0, 0));
                display.show(JAS_DISPLEJE);
            }

            // Přejdeme do dalšího stavu
            gStav = 2;
        } else if (gStav == 3) { // zkontrolujeme zadanou sekvenci a vyblikáme výsledek
            // Kontrola sekvence, kterou zadal uživatel proti vzoru
            bool correct = true;
            for (int s = 0; s < gKroky; ++s) {
                if (gSekvenceZadana[s] != gSekvencePredloha[s]) {
                    correct = false;
                    break;
                }
            }

            // Vyblikáme výsledek
            bool on = true;
            for (int i = 0; i < 6; ++i) {
                auto clr = Rgb(0, 0, 0);
                if (on) {
                    if (correct) {
                        clr = Rgb(0, 255, 0);
                    } else {
                        clr = Rgb(255, 0, 0);
                    }
                }

                for (const auto& t : dlazdice) {
                    display.nakresliObdelnikVyplneny(t.obdelnik, clr);
                }
                display.ukaz(JAS_DISPLEJE);

                on = !on;
                cekejVteriny(0.15);
            }

            // Pro další hru zvýšíme obtížnost, přidáme další prvek do sekvence
            if (correct) {
                gKroky += 1;
            }

            // Vyčistíme sekvence pro další hru
            gSekvenceZadana.clear();

            // Přejdeme zpět do stavu 1, kdy vyblikáváme další sekvenci.
            gStav = 1;
            cekejVteriny(0.5);
        }

        vTaskDelay(10);
    }
}

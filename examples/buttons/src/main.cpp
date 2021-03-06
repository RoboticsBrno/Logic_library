#include "Logic.hpp"
#include <iostream>

void logicMain() {

    buttons.onChange([](std::bitset<MaxID> currentState, std::bitset<MaxID> buttonChange) {
        if (buttonChange[Up] == 1) {

            std::cout << "Button Up has changed and is now ";

            if (currentState[Up] == 1) {
                std::cout << "pressed." << std::endl;
            } else {
                std::cout << "released." << std::endl;
            }
        }
    });

    buttons.onChange([](std::bitset<MaxID> currentState) {
        std::cout << "Button Up is now ";

        if (currentState[Up] == 1) {
            std::cout << "pressed." << std::endl;
        } else {
            std::cout << "released." << std::endl;
        }
    });

    buttons.onChange([]() {
        std::cout << "A button has changed. No more information available" << std::endl;
    });

    buttons.onChange([](std::bitset<MaxID> currentState, std::bitset<MaxID> buttonChange) {
        std::cout << "You will see this only when Button Down changes, and the button is now ";

        if (currentState[Down] == 1) {
            std::cout << "pressed." << std::endl;
        } else {
            std::cout << "released." << std::endl;
        }
    },
        Down);

    buttons.onChange([](bool isPressed) {
        std::cout << "You will see this only when Button Down changes" << std::endl;

        if (isPressed == 1) {
            std::cout << "pressed." << std::endl;
        } else {
            std::cout << "released." << std::endl;
        }
    },
        Down);

    buttons.onChange([]() {
        std::cout << "You will see this only when Button Down changes" << std::endl;
    },
        Down);

    buttons.onPress([](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
        std::cout << "A button has been pressed. ";

        if (changed[Right] == 1) {
            std::cout << "It was the Right button" << std::endl;
        } else {
            std::cout << "It was not the Right button" << std::endl;
        }
    });

    buttons.onPress([](std::bitset<MaxID> currentState) {
        std::cout << "A button has been pressed. ";

        std::cout << "Buttons ";

            for (int i = 0; i < MaxID; i++) {
            if (currentState[i] == 1) {
                std::cout << i << ", ";
            }
        }

        std::cout << "are now pressed." << std::endl;
    });

    buttons.onPress([]() {
        std::cout << "A button has been pressed. No more information available" << std::endl;
    });

    buttons.onPress([]() {
        std::cout << "You will see this only when Button Left get pressed down." << std::endl;
    },
        Left);

    buttons.onRelease([](std::bitset<MaxID> currentState, std::bitset<MaxID> changed) {
        std::cout << "A button has been released. ";

        if (changed[Right] == 1) {
            std::cout << "It was the Right button" << std::endl;
        } else {
            std::cout << "It was not the Right button" << std::endl;
        }
    });

    buttons.onRelease([](std::bitset<MaxID> currentState) {
        std::cout << "A button has been released. ";

        std::cout << "Buttons ";

            for (int i = 0; i < MaxID; i++) {
            if (currentState[i] == 0) {
                std::cout << i << ", ";
            }
        }

        std::cout << "are now released." << std::endl;
    });

    buttons.onRelease([]() {
        std::cout << "A button has been released. No more information available" << std::endl;
    });

    buttons.onRelease([]() {
        std::cout << "You will see this only when Button Left gets released." << std::endl;
    },
        Left);

    buttons.priZmene([](std::bitset<MaxID> aktualniStav, std::bitset<MaxID> zmenenaTlacitka) {
        if (zmenenaTlacitka[Up] == 1) {

            std::cout << "Tla????tko Nahoru se zm??nilo a te?? je ";

            if (aktualniStav[Up] == 1) {
                std::cout << "zm????knut??." << std::endl;
            } else {
                std::cout << "pu??t??n??." << std::endl;
            }
        }
    });

    buttons.priZmene([](std::bitset<MaxID> aktualniStav) {
        std::cout << "Tla????tko Nahoru je nyn?? ";

        if (aktualniStav[Up] == 1) {
            std::cout << "zm????knut??." << std::endl;
        } else {
            std::cout << "pu??t??n??." << std::endl;
        }
    });

    buttons.priZmene([]() {
        std::cout << "Tla????tko bylo zm??n??no. Nen?? k dispozici v??ce informac??." << std::endl;
    });

    buttons.priZmene([](std::bitset<MaxID> aktualniStav, std::bitset<MaxID> zmenenaTlacitka) {
        std::cout << "Tohle uvid??te pouze pokud se zm??n?? tla????tko Dol??, a toto tla????tko je nyn?? ";

        if (aktualniStav[Down] == 1) {
            std::cout << "zm????knut??." << std::endl;
        } else {
            std::cout << "pu??t??n??." << std::endl;
        }
    },
        Down);

    buttons.priZmene([](bool isPressed) {
        std::cout << "Tohle uvid??te pouze pokud se zm??n?? tla????tko Dol??" << std::endl;

        if (isPressed == 1) {
            std::cout << "zm????knut??." << std::endl;
        } else {
            std::cout << "pu??t??n??." << std::endl;
        }
    },
        Down);

    buttons.onChange([]() {
        std::cout << "Tohle uvid??te pouze pokud se zm??n?? tla????tko Dol??" << std::endl;
    },
        Down);

    buttons.onPress([](std::bitset<MaxID> aktualniStav, std::bitset<MaxID> zmena) {
        std::cout << "N??jak?? tla????tko bylo zm????knuto. ";

        if (zmena[Right] == 1) {
            std::cout << "Bylo to tla????tko Vpravo" << std::endl;
        } else {
            std::cout << "Nebylo to tla????tko Vpravo" << std::endl;
        }
    });

    buttons.onPress([](std::bitset<MaxID> aktualniStav) {
        std::cout << "N??jak?? tla????tko bylo zm????knuto. ";

        std::cout << "Tla????tka ";

            for (int i = 0; i < MaxID; i++) {
            if (aktualniStav[i] == 1) {
                std::cout << i << ", ";
            }
        }

        std::cout << "jsou nyn?? zm????knut??." << std::endl;
    });

    buttons.onPress([]() {
        std::cout << "Tla????tko bylo Zm????knuto. Nen?? k dispozici v??ce informac??." << std::endl;
    });

    buttons.onPress([]() {
        std::cout << "Tohle uvid??te pouze pokud zm????knete tla????tko Vlevo." << std::endl;
    },
        Left);

    buttons.priUvolneni([](std::bitset<MaxID> aktualniStav, std::bitset<MaxID> zmena) {
        std::cout << "Tla????tko bylo pu??t??no. ";

        if (zmena[Right] == 1) {
            std::cout << "Bylo to tla????tko Vpravo" << std::endl;
        } else {
            std::cout << "Nebylo to tla????tko Vpravo" << std::endl;
        }
    });

    buttons.priUvolneni([](std::bitset<MaxID> aktualniStav) {
        std::cout << "Tla????tko bylo pu??t??no. ";

        std::cout << "Tla????tka ";

            for (int i = 0; i < MaxID; i++) {
            if (aktualniStav[i] == 0) {
                std::cout << i << ", ";
            }
        }

        std::cout << "jsou nyn?? pu??t??n??." << std::endl;
    });

    buttons.priUvolneni([]() {
        std::cout << "Tla????tko bylo uvoln??no. Nen?? k dispozici v??ce informac??." << std::endl;
    });

    buttons.priUvolneni([]() {
        std::cout << "Tohle uvid??te pouze pokud pust??te tla????tko Vlevo." << std::endl;
    },
        Left);
}
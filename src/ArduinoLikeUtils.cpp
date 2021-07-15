#include "ArduinoLikeUtils.hpp"
#include "Logic.hpp"

void saveInt(const std::string& name, int value) {
    logic.nvs().writeInt(name.c_str(), value);
    logic.nvs().commit();
}

int loadInt(const std::string& name, int defaultVal) {
    if (logic.nvs().existsInt(name.c_str())) {
        return logic.nvs().getInt(name.c_str());
    }
    return defaultVal;
}

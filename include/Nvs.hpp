#pragma once

#include <string>

#include <esp_system.h>
#include <nvs.h>
#include <nvs_flash.h>

class Nvs {
public:
    Nvs(const char* name_space, const char* partition = "nvs");
    ~Nvs();
    Nvs(const Nvs&) = delete;

    bool existsInt(const char* key);
    std::int32_t getInt(const char* key);
    void writeInt(const char* key, std::int32_t value);

    bool existsString(const char* key);
    std::string getString(const char* key);
    void writeString(const char* key, const std::string& value);

    void commit();

private:
    esp_err_t initFlash();

    nvs_handle m_handle;
    bool m_dirty;
};

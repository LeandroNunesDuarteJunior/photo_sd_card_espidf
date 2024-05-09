#pragma once

#include "esp_system.h"
#include <string>

void mount_sdcard_filesystem(void);
void umount_sdcard_filesystem(void);
esp_err_t write_file(std::string const &path, uint8_t *data, size_t data_length);
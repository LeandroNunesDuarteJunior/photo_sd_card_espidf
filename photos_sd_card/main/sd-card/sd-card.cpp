#include "sd-card.hpp"
#include "sdmmc_cmd.h"
#include "esp_log.h"
#include <esp_vfs_fat.h>
#include "driver/sdmmc_host.h"


const char *FAT_MOUNT_PATH = "/sdcard";
sdmmc_card_t *ptr_global_card = nullptr;

void mount_sdcard_filesystem() {
    // Configure SDMMC host
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    // Configure SDMMC slot
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    // Configure FAT filesystem
    esp_vfs_fat_mount_config_t mount_config = {
            .max_files = 5,
            .allocation_unit_size = 8 * 1024
    };

    // Mount the SD card and initialize FAT filesystem
    esp_err_t err = esp_vfs_fat_sdmmc_mount(FAT_MOUNT_PATH, &host, &slot_config, &mount_config, &ptr_global_card);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Failed to mount SD card: %s\n", esp_err_to_name(err));
        return;
    }
    ESP_LOGW(__func__, "SD Card successfully mounted\n");

    sdmmc_card_print_info(stdout, ptr_global_card);

}


void umount_sdcard_filesystem() {
    // Unmount the SD card (optional)
    esp_err_t err = esp_vfs_fat_sdcard_unmount(FAT_MOUNT_PATH, ptr_global_card);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Failed to unmount SD card: %s\n", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(__func__, "SD card unmounted successfully!\n");
}

esp_err_t write_file(std::string const &path, uint8_t *data, size_t data_length) {
    ESP_LOGW(__func__, "Opening file %s", path.c_str());
    std::string fullPath = std::string(FAT_MOUNT_PATH) + std::string("/images/") + path;

//    ESP_LOGW(__func__, "write full path %s\n", fullPath.c_str());
    FILE *f = fopen(fullPath.c_str(), "w");
    if (f == nullptr) {
        ESP_LOGE(__func__, "Failed to open file for writing");
        return ESP_FAIL;
    }
    ESP_LOGW(__func__, "File opened");
    size_t written_elems = fwrite(data, data_length, 1, f);
    if (written_elems != 1) {
        ESP_LOGE(__func__, " error writing to file");
        ESP_LOGE(__func__, "Fail: only wrote %d elems\n", written_elems);
    }
    fclose(f);
//    ESP_LOGW(__func__, "File written");

    return ESP_OK;
}
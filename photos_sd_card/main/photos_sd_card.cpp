#include <stdio.h>
#include "camera/camera.hpp"

#include "freertos/FreeRTOS.h"
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "freertos/queue.h"
#include "esp_task.h"

#include "sd-card/sd-card.hpp"
#include <string>

esp_err_t write_frame(uint8_t * rgb888_buf, size_t width, size_t height, pixformat_t pix_format) {
    uint8_t * jpg_buf = NULL;
    size_t jpg_buf_len = 0;
    //esp_err_t res = ESP_OK;

    bool res = fmt2jpg(rgb888_buf, width * height, width, height, pix_format, 80, &jpg_buf, &jpg_buf_len);
    if (res == false) {
        ESP_LOGE(__func__, "JPEG compression failed !!!!");
        return res;
    }
    
    std::string file_name = "b_";
    //file_name.append(std::string(buf,strlen(buf))); 
    file_name = file_name.substr(0,8) +".jpg" ;
    ESP_LOGI(__func__, "%s" , file_name.c_str());
    write_file(file_name, jpg_buf, jpg_buf_len);

    free(jpg_buf);

    return res;
}


extern "C" void app_main(void)
{
    camera camera;
    if (!camera.init()) {
        ESP_LOGE(TAG, "Camera initialization failed");
    }
    camera_fb_t * fb = camera.capture();
    if (!fb) {
        ESP_LOGE(TAG, "Camera capture failed");

    }else{
        ESP_LOGI(TAG, "Camera captured");
    }

    mount_sdcard_filesystem();
    write_frame(reinterpret_cast<uint8_t*>(fb), 400, 296, PIXFORMAT_GRAYSCALE);

}

#pragma once

#include "freertos/FreeRTOS.h"
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "freertos/queue.h"
#include "esp_task.h"

#include "esp_camera.h"
#include "esp_log.h"

#define TAG "Camera"

class camera{
    public:
    // Constructor e destrutor
    camera();
    ~camera();
    //Métodos da classe:
    //Retorno boleano para inicialização
    bool init();
    //Função de captura de um frameBuffer da câmera do tipo ponteiro "camera_fb_t"
    camera_fb_t* capture();
    private:
    camera_config_t camera_config;
};
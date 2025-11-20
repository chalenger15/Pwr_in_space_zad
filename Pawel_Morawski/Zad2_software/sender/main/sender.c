#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/twai.h"

static const char *TAG = "CAN_SENDER";

#define TWAI_TX_GPIO 21
#define TWAI_RX_GPIO 22

static void init_twai(void) {
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TWAI_TX_GPIO, TWAI_RX_GPIO, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_ERROR_CHECK(twai_start());
    ESP_LOGI(TAG, "Interfejs skonfigurowany (nadawca)");
}

static void send_can_message(uint16_t id, const uint8_t *data, uint8_t dlc) {
    twai_message_t message = {
        .identifier = id,
        .data_length_code = dlc,
    };
    memcpy(message.data, data, dlc);
    twai_status_info_t status;
    ESP_ERROR_CHECK(twai_get_status_info(&status));
    esp_err_t err = twai_transmit(&message, pdMS_TO_TICKS(1000));
    if (err == ESP_OK)  {
        ESP_LOGI(TAG, "Wyslano ID = 0x%2X, dlc = %d", id, dlc);
    }   else{
        ESP_LOGW(TAG, "Blad wysylania ID = 0x%2X, err = %d", id, err);
    }
}

void app_main(void)
{
    init_twai();

    const TickType_t delay_ms = pdMS_TO_TICKS(500);


    uint8_t data1[1] = {0xAA};
    uint8_t data4[4] = {0x54, 0xB9, 0xFF, 0xAB};
    uint8_t data8[8] = {0x66, 0x78, 0x43, 0x03, 0x6D, 0xAB, 0xEF, 0xDD};

    while (1)   {
        uint16_t id;
        switch (rand()%4)   {
            case 0:
                id = 0xB0;
                break;
            case 1:
                id = 0x71;
                break;
            case 2:
                id = 0x92;
                break;
            case 3:
                id = 0x23;
                break;
            default:
                id = 0x00;
        }
        int x = rand()%3;
        if (x == 0)    {
            send_can_message(id, data4, 4);
            vTaskDelay(delay_ms);
        }   else if (x == 1) {
            send_can_message(id, data1, 1);
            vTaskDelay(delay_ms);
        }   else{
            send_can_message(id, data8, 8);
            vTaskDelay(delay_ms);
        }
    }
}
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/twai.h"
#include "esp_timer.h"

static const char *TAG = "CAN_RECEIVER";

#define TWAI_TX_GPIO 21
#define TWAI_RX_GPIO 22

static void init_twai(void) {
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TWAI_TX_GPIO, TWAI_RX_GPIO, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_ERROR_CHECK(twai_start());
    ESP_LOGI(TAG, "Interfejs skonfigurowany (odbiornik)");
}

static void print_hex(const uint8_t *buf, uint8_t len)  {
    for (int i = 0; i < len; i++)   {
        printf("odczyt z komponentu %d: %d", i+1, (int)buf[i]);
    }
}

 char* priority_from_id(uint32_t id)    {
    int id_dec = (int)id;
    if (id_dec/16 > 10) {
        return "WYSOKI";
    }   else if (id_dec/16 > 5){
        return "SREDNI";
    }   else{
        return "NISKI";
    }
}

char* data_type_from_id(uint32_t id)    {
    int id_dec = (int)id;
    switch(id_dec%16)   {
        case 0:
            return "Temperatura";
            break;
        case 1:
            return "Cisnienie";
            break;
        case 2:
            return "Status systemu";
            break;
        case 3:
            return "Przyspieszenie";
            break;
        default:
            ESP_LOGI(TAG, "Nieznany typ danych (blad identyfikacji)");
            return " ";
    }
}

void app_main(void)
{
    init_twai();
    twai_message_t rx_msg;

    while (1)   {
        esp_err_t err = twai_receive(&rx_msg, pdMS_TO_TICKS(1000));
        if (err == ESP_OK)  {
            int64_t ts = esp_timer_get_time()/1000;
            ESP_LOGI(TAG, "Odebrano");
            printf("Dane: %s ", data_type_from_id(rx_msg.identifier));
            print_hex(rx_msg.data, rx_msg.data_length_code);
            printf("Priorytet: %s, Czas(ms) : %lld", priority_from_id(rx_msg.identifier), ts);
        } else if (err == ESP_ERR_TIMEOUT) {
            ESP_LOGI(TAG, "brak wiadomosci w przedziale czasu");
        }
    }
}

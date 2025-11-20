#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_vfs_fat.h"
#include "driver/spi_common.h"
#include "driver/sdspi_host.h"
#include "esp_log.h"
#include "sdmmc_cmd.h"

static const char *TAG = "SD_CSV";


#define PIN_NUM_CLK 18
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CS 5
#define LICZBA_DANYCH 6
char* nazwy_danych = "czas,temperatura,cisnienie,przyspieszenie,kat_nachylenia,wysokosc";


void app_main(void)
{
    const TickType_t delay_ms = pdMS_TO_TICKS(20);

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 6,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t *card;
    const char mount_point[] = "/sdcard";
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();

    spi_bus_config_t bus_config = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 256,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(host.slot, &bus_config, SDSPI_DEFAULT_DMA));

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;
    esp_err_t ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK)  {
        ESP_LOGE(TAG, "Blad dostepu do karty SD");
        return;
    }   
    ESP_LOGI(TAG, "Karta SD dziala");

    FILE *f = fopen("/sdcard/dane.csv", "w");
    if (f == NULL)   {
        ESP_LOGE(TAG, "Blad otwarcia pliku");
        return;
    }

    fprintf(f, "%s\n", nazwy_danych);
    int i;
    while (1)   {
        for (i = 0; i < LICZBA_DANYCH; i++) {
            fprintf(f, "%d", rand()%444);
            if (i+1 < LICZBA_DANYCH)    {
                fprintf(f, ",");
            }
        } 
        fprintf(f, "\n");
        vTaskDelay(delay_ms);
        
    
    }
}

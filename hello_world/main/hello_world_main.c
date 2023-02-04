#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_err.h>
#include <esp_idf_version.h>
#include <max31855.h>
#include <string.h>

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(4, 0, 0)
#define HOST    HSPI_HOST
#else
#define HOST    SPI2_HOST
#endif

#define MISO_IO_NUM 12
#define SCLK_IO_NUM 14

int CS_IO_NUM[] = {
    15,     // sensor 1
    16      // sensor 5
};
// int CS_IO_NUM[] = {
//     15,     // sensor 1
//     2,      // sensor 2
//     0,      // sensor 3
//     4,      // sensor 4
//     16,     // sensor 5
//     17,     // sensor 6
//     5,      // sensor 7 
//     1       // sensor 8
// };

static const char *TAG = "max31855-example";

static void task(void *pvParameter)
{
    // Configure SPI bus
//for(int i = 0; i<8; i++){
    spi_bus_config_t cfg = {
       .mosi_io_num = -1,
       .miso_io_num = MISO_IO_NUM,
       .sclk_io_num = SCLK_IO_NUM,
       .quadwp_io_num = -1,
       .quadhd_io_num = -1,
       .max_transfer_sz = 0,
       .flags = 0
    };

    ESP_ERROR_CHECK(spi_bus_initialize(HOST, &cfg, 1));

    int numOfSensor = sizeof(CS_IO_NUM)/sizeof(int);
    max31855_t dev[numOfSensor];
    memset(&dev, 0, sizeof(max31855_t)*numOfSensor);
    for(int i = 0; i < numOfSensor; i++) {
        ESP_ERROR_CHECK(max31855_init_desc(&dev[i], HOST, MAX31855_MAX_CLOCK_SPEED_HZ, CS_IO_NUM[i]));
    }
    // Init device

    float tc_t, cj_t;
    bool scv, scg, oc;
    while (1)
    {
        for(int i = 0; i < numOfSensor; i++) {
        esp_err_t res = max31855_get_temperature(&dev[i], &tc_t, &cj_t, &scv, &scg, &oc);
        if (res != ESP_OK)
            ESP_LOGE(TAG, "Failed to measure: %d (%s)", res, esp_err_to_name(res));
        else
        {
            if (scv) ESP_LOGW(TAG, "Thermocouple shorted to VCC!");
            if (scg) ESP_LOGW(TAG, "Thermocouple shorted to GND!");
            if (oc) ESP_LOGW(TAG, "No connection to thermocouple!");
            ESP_LOGI(TAG, "%d Temperature: %.2f°C, cold junction temperature: %.4f°C", i, tc_t, cj_t);
        }

        vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}

void app_main()
{
    xTaskCreate(task, TAG, configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);
}
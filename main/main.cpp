#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_vfs_dev.h"
#include "modbus.h"

#define TAG "RS485_ECHO_APP"

#define ECHO_TEST_TXD (32)
#define ECHO_TEST_RXD (35)

#define BUF_SIZE (127)

void process_input_registers(uint16_t *input_registers, int length)
{
    printf("Device Type: %d\n", input_registers[0]);        // IO_DEVICE_TYPE
    printf("Data Model Version: %d\n", input_registers[1]); // IO_DATAMODEL_VERSION
    printf("Operational Mode: %d\n", input_registers[3]);   // IO_OPERATIONAL_MODE

    // Đọc Sensor Data
    printf("Sensor Data (Sensor 1): %d\n", input_registers[5]); // IO_SENSOR1_DATA

    // Đọc nhiệt độ của Panel
    printf("Panel Temperature: %d.%d°C\n", input_registers[8] / 10, input_registers[8] % 10); // IO_PANEL_TEMPERATURE

    // Đọc nguồn điện ngoài
    printf("External Power Voltage: %d.%dV\n", input_registers[9] / 10, input_registers[9] % 10); // IO_EXT_POWER_SENSOR

    // Đọc ngày hiệu chuẩn từ Floating Point (Register 332 & 333)
    uint32_t calibration_date = (input_registers[332] << 16) | input_registers[333];
    uint8_t year = (calibration_date >> 24) & 0xFF;
    uint8_t month = (calibration_date >> 16) & 0xFF;
    uint8_t day = (calibration_date >> 8) & 0xFF;
    printf("Calibration Date: %02d/%02d/%02d\n", year, month, day); // Format: YY/MM/DD
}

// Function to read Modbus registers
static void reading_registers_of_slave(void *arg)
{
    // UART configuration
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Install UART driver
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, 2048, 256, 0, NULL, 0));
    esp_vfs_dev_uart_set_rx_line_endings(ESP_LINE_ENDINGS_LF);
    esp_vfs_dev_uart_set_tx_line_endings(ESP_LINE_ENDINGS_LF);

    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_set_mode(UART_NUM_1, UART_MODE_RS485_HALF_DUPLEX));

    // Initialize Modbus context
    modbus_t *ctx = modbus_new_rtu("/dev/uart/1", 9600, 'N', 8, 1);
    if (ctx == NULL)
    {
        ESP_LOGE(TAG, "Unable to create the libmodbus context");
        vTaskDelete(NULL);
    }

    // Set Modbus configurations
    modbus_set_slave(ctx, 99); // modbus ID
    modbus_set_response_timeout(ctx, 2, 0);
    modbus_set_debug(ctx, TRUE);

    // Attempt to connect Modbus
    if (modbus_connect(ctx) == -1)
    {
        ESP_LOGE(TAG, "Modbus connection failed");
        modbus_free(ctx);
        vTaskDelete(NULL);
    }

    uint16_t buff[10]; // Buffer for holding register values

    while (1)
    {
        int len_modbus = modbus_read_input_registers(ctx, 0, 10, buff);
        if (len_modbus < 0)
        {
            ESP_LOGE(TAG, "Failed to read Modbus registers");
        }
        else
        {
            ESP_LOGI(TAG, "Modbus data received:");
            process_input_registers(buff, 10); // Xử lý dữ liệu từ các thanh ghi Input
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }

    // Clean up resources
    modbus_close(ctx);
    modbus_free(ctx);
    vTaskDelete(NULL);
}

// Main application entry point
extern "C" void app_main(void)
{
    xTaskCreate(reading_registers_of_slave, "modbus_task", 4096, NULL, 10, NULL);
}

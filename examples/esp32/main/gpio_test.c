#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "gpio_test";

#define LED_PIN 2
#define BUTTON_PIN 0

void app_main(void)
{
    ESP_LOGI(TAG, "GPIO Test Application Starting");
    
    // Configure LED pin as output
    gpio_config_t led_config = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&led_config);
    
    // Configure button pin as input
    gpio_config_t button_config = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&button_config);
    
    ESP_LOGI(TAG, "GPIO Test Application Initialized");
    
    while (1) {
        // Read button state
        int button_state = gpio_get_level(BUTTON_PIN);
        
        // Toggle LED based on button state
        if (button_state == 0) {
            gpio_set_level(LED_PIN, 1);
            ESP_LOGI(TAG, "Button pressed - LED ON");
        } else {
            gpio_set_level(LED_PIN, 0);
            ESP_LOGI(TAG, "Button released - LED OFF");
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
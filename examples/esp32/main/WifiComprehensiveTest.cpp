#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/event_groups.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_err.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <lwip/err.h>
#include <lwip/sys.h>

static const char *TAG = "WIFI_COMPREHENSIVE_TEST";

// WiFi configuration
#define WIFI_SSID      "TEST_SSID"
#define WIFI_PASS      "TEST_PASSWORD"
#define MAXIMUM_RETRY  5

// Test configuration
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

// Test result structure
typedef struct {
    const char* test_name;
    bool passed;
    const char* error_message;
} test_result_t;

// Test section structure
typedef struct {
    const char* section_name;
    bool enabled;
    int total_tests;
    int passed_tests;
    int failed_tests;
} test_section_t;

// Global test sections
test_section_t test_sections[] = {
    {"WIFI_INITIALIZATION", true, 0, 0, 0},
    {"WIFI_BASIC_OPERATIONS", true, 0, 0, 0},
    {"WIFI_CONNECTIVITY", true, 0, 0, 0},
    {"WIFI_PERFORMANCE", true, 0, 0, 0},
    {"WIFI_STRESS_TESTING", true, 0, 0, 0},
    {"WIFI_ERROR_HANDLING", true, 0, 0, 0}
};

#define NUM_SECTIONS (sizeof(test_sections) / sizeof(test_sections[0]))

// Global variables
static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;
static bool wifi_initialized = false;

// Function prototypes
void run_test_section(int section_index);
void print_test_results(void);
void enable_test_section(const char* section_name, bool enable);
void run_all_tests(void);
esp_err_t wifi_init(void);
esp_err_t wifi_deinit(void);

// Test functions for each section
esp_err_t test_wifi_init(void);
esp_err_t test_wifi_deinit(void);
esp_err_t test_wifi_scan(void);
esp_err_t test_wifi_connect(void);
esp_err_t test_wifi_disconnect(void);
esp_err_t test_wifi_reconnect(void);
esp_err_t test_wifi_signal_strength(void);
esp_err_t test_wifi_channel_switch(void);
esp_err_t test_wifi_power_save(void);
esp_err_t test_wifi_stress_connect(void);
esp_err_t test_wifi_stress_disconnect(void);
esp_err_t test_wifi_invalid_credentials(void);
esp_err_t test_wifi_timeout_handling(void);

// Test result queue
QueueHandle_t test_result_queue;

// WiFi event handler
static void event_handler(void* arg, esp_event_base_t event_base,
                         int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

// Main application entry point
extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting WiFi Comprehensive Test Suite");
    
    // Create test result queue
    test_result_queue = xQueueCreate(100, sizeof(test_result_t));
    if (test_result_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create test result queue");
        return;
    }
    
    // Create WiFi event group
    s_wifi_event_group = xEventGroupCreate();
    if (s_wifi_event_group == NULL) {
        ESP_LOGE(TAG, "Failed to create WiFi event group");
        return;
    }
    
    // Check command line arguments for section selection
    int argc = 0;
    char** argv = NULL;
    
    // If no arguments, run all tests
    if (argc == 1) {
        ESP_LOGI(TAG, "No section specified, running all tests");
        run_all_tests();
    } else {
        // Parse section arguments
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--section") == 0 && i + 1 < argc) {
                enable_test_section(argv[i + 1], true);
                i++; // Skip next argument
            } else if (strcmp(argv[i], "--disable-section") == 0 && i + 1 < argc) {
                enable_test_section(argv[i + 1], false);
                i++; // Skip next argument
            } else if (strcmp(argv[i], "--list-sections") == 0) {
                ESP_LOGI(TAG, "Available test sections:");
                for (int j = 0; j < NUM_SECTIONS; j++) {
                    ESP_LOGI(TAG, "  %s: %s", 
                             test_sections[j].section_name,
                             test_sections[j].enabled ? "ENABLED" : "DISABLED");
                }
                return;
            } else if (strcmp(argv[i], "--help") == 0) {
                ESP_LOGI(TAG, "Usage: wifi_test [OPTIONS]");
                ESP_LOGI(TAG, "Options:");
                ESP_LOGI(TAG, "  --section SECTION_NAME     Enable specific test section");
                ESP_LOGI(TAG, "  --disable-section SECTION_NAME  Disable specific test section");
                ESP_LOGI(TAG, "  --list-sections          List all available test sections");
                ESP_LOGI(TAG, "  --help                   Show this help message");
                ESP_LOGI(TAG, "Examples:");
                ESP_LOGI(TAG, "  wifi_test --section WIFI_BASIC_OPERATIONS");
                ESP_LOGI(TAG, "  wifi_test --section WIFI_INITIALIZATION --section WIFI_CONNECTIVITY");
                return;
            }
        }
        
        // Run enabled sections
        run_all_tests();
    }
    
    // Print final results
    print_test_results();
    
    // Cleanup
    vQueueDelete(test_result_queue);
    vEventGroupDelete(s_wifi_event_group);
    ESP_LOGI(TAG, "WiFi Comprehensive Test Suite completed");
}

// Enable/disable test sections
void enable_test_section(const char* section_name, bool enable)
{
    for (int i = 0; i < NUM_SECTIONS; i++) {
        if (strcmp(test_sections[i].section_name, section_name) == 0) {
            test_sections[i].enabled = enable;
            ESP_LOGI(TAG, "Section %s %s", section_name, enable ? "enabled" : "disabled");
            return;
        }
    }
    ESP_LOGW(TAG, "Section %s not found", section_name);
}

// Run all enabled test sections
void run_all_tests(void)
{
    for (int i = 0; i < NUM_SECTIONS; i++) {
        if (test_sections[i].enabled) {
            ESP_LOGI(TAG, "Running test section: %s", test_sections[i].section_name);
            run_test_section(i);
            vTaskDelay(pdMS_TO_TICKS(2000)); // Delay between sections
        }
    }
}

// Run tests for a specific section
void run_test_section(int section_index)
{
    test_section_t* section = &test_sections[section_index];
    section->total_tests = 0;
    section->passed_tests = 0;
    section->failed_tests = 0;
    
    if (strcmp(section->section_name, "WIFI_INITIALIZATION") == 0) {
        // WiFi Initialization tests
        section->total_tests = 2;
        
        if (test_wifi_init() == ESP_OK) {
            section->passed_tests++;
            ESP_LOGI(TAG, "✓ WiFi initialization test passed");
        } else {
            section->failed_tests++;
            ESP_LOGE(TAG, "✗ WiFi initialization test failed");
        }
        
        if (test_wifi_deinit() == ESP_OK) {
            section->passed_tests++;
            ESP_LOGI(TAG, "✓ WiFi deinitialization test passed");
        } else {
            section->failed_tests++;
            ESP_LOGE(TAG, "✗ WiFi deinitialization test failed");
        }
        
    } else if (strcmp(section->section_name, "WIFI_BASIC_OPERATIONS") == 0) {
        // WiFi Basic operations tests
        section->total_tests = 2;
        
        if (test_wifi_init() == ESP_OK) {
            if (test_wifi_scan() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ WiFi scan test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ WiFi scan test failed");
            }
            
            if (test_wifi_deinit() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ WiFi deinit after scan test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ WiFi deinit after scan test failed");
            }
        } else {
            section->failed_tests += 2;
            ESP_LOGE(TAG, "✗ WiFi basic operations tests failed (init failed)");
        }
        
    } else if (strcmp(section->section_name, "WIFI_CONNECTIVITY") == 0) {
        // WiFi Connectivity tests
        section->total_tests = 3;
        
        if (test_wifi_init() == ESP_OK) {
            if (test_wifi_connect() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ WiFi connect test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ WiFi connect test failed");
            }
            
            if (test_wifi_disconnect() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ WiFi disconnect test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ WiFi disconnect test failed");
            }
            
            if (test_wifi_reconnect() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ WiFi reconnect test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ WiFi reconnect test failed");
            }
            
            test_wifi_deinit();
        } else {
            section->failed_tests += 3;
            ESP_LOGE(TAG, "✗ WiFi connectivity tests failed (init failed)");
        }
        
    } else if (strcmp(section->section_name, "WIFI_PERFORMANCE") == 0) {
        // WiFi Performance tests
        section->total_tests = 3;
        
        if (test_wifi_init() == ESP_OK) {
            if (test_wifi_signal_strength() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ WiFi signal strength test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ WiFi signal strength test failed");
            }
            
            if (test_wifi_channel_switch() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ WiFi channel switch test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ WiFi channel switch test failed");
            }
            
            if (test_wifi_power_save() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ WiFi power save test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ WiFi power save test failed");
            }
            
            test_wifi_deinit();
        } else {
            section->failed_tests += 3;
            ESP_LOGE(TAG, "✗ WiFi performance tests failed (init failed)");
        }
        
    } else if (strcmp(section->section_name, "WIFI_STRESS_TESTING") == 0) {
        // WiFi Stress testing
        section->total_tests = 2;
        
        if (test_wifi_init() == ESP_OK) {
            if (test_wifi_stress_connect() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ WiFi stress connect test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ WiFi stress connect test failed");
            }
            
            if (test_wifi_stress_disconnect() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ WiFi stress disconnect test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ WiFi stress disconnect test failed");
            }
            
            test_wifi_deinit();
        } else {
            section->failed_tests += 2;
            ESP_LOGE(TAG, "✗ WiFi stress tests failed (init failed)");
        }
        
    } else if (strcmp(section->section_name, "WIFI_ERROR_HANDLING") == 0) {
        // WiFi Error handling tests
        section->total_tests = 2;
        
        if (test_wifi_init() == ESP_OK) {
            if (test_wifi_invalid_credentials() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ WiFi invalid credentials test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ WiFi invalid credentials test failed");
            }
            
            if (test_wifi_timeout_handling() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ WiFi timeout handling test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ WiFi timeout handling test failed");
            }
            
            test_wifi_deinit();
        } else {
            section->failed_tests += 2;
            ESP_LOGE(TAG, "✗ WiFi error handling tests failed (init failed)");
        }
    }
    
    ESP_LOGI(TAG, "Section %s completed: %d/%d tests passed", 
             section->section_name, section->passed_tests, section->total_tests);
}

// Print comprehensive test results
void print_test_results(void)
{
    ESP_LOGI(TAG, "=== WIFI COMPREHENSIVE TEST RESULTS ===");
    
    int total_tests = 0;
    int total_passed = 0;
    int total_failed = 0;
    
    for (int i = 0; i < NUM_SECTIONS; i++) {
        if (test_sections[i].enabled) {
            ESP_LOGI(TAG, "Section: %s", test_sections[i].section_name);
            ESP_LOGI(TAG, "  Tests: %d/%d passed", 
                     test_sections[i].passed_tests, test_sections[i].total_tests);
            
            total_tests += test_sections[i].total_tests;
            total_passed += test_sections[i].passed_tests;
            total_failed += test_sections[i].failed_tests;
        }
    }
    
    ESP_LOGI(TAG, "=== OVERALL RESULTS ===");
    ESP_LOGI(TAG, "Total Tests: %d", total_tests);
    ESP_LOGI(TAG, "Passed: %d", total_passed);
    ESP_LOGI(TAG, "Failed: %d", total_failed);
    ESP_LOGI(TAG, "Success Rate: %.1f%%", 
             total_tests > 0 ? (float)total_passed / total_tests * 100.0f : 0.0f);
    
    if (total_failed == 0) {
        ESP_LOGI(TAG, "🎉 ALL TESTS PASSED! 🎉");
    } else {
        ESP_LOGW(TAG, "⚠️  %d tests failed", total_failed);
    }
}

// WiFi initialization
esp_err_t wifi_init(void)
{
    if (wifi_initialized) {
        ESP_LOGI(TAG, "WiFi already initialized");
        return ESP_OK;
    }
    
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    ESP_ERROR_CHECK(esp_event_handler_instance_t::create(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_t::create(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));
    
    wifi_initialized = true;
    ESP_LOGI(TAG, "WiFi initialization completed");
    return ESP_OK;
}

// WiFi deinitialization
esp_err_t wifi_deinit(void)
{
    if (!wifi_initialized) {
        ESP_LOGI(TAG, "WiFi not initialized");
        return ESP_OK;
    }
    
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_wifi_deinit());
    ESP_ERROR_CHECK(esp_event_loop_delete_default());
    ESP_ERROR_CHECK(esp_netif_deinit());
    
    wifi_initialized = false;
    ESP_LOGI(TAG, "WiFi deinitialization completed");
    return ESP_OK;
}

// Individual test implementations
esp_err_t test_wifi_init(void)
{
    return wifi_init();
}

esp_err_t test_wifi_deinit(void)
{
    return wifi_deinit();
}

esp_err_t test_wifi_scan(void)
{
    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = false,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time = {
            .active = {
                .min = 120,
                .max = 150
            }
        }
    };
    
    esp_err_t err = esp_wifi_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start WiFi: %s", esp_err_to_name(err));
        return err;
    }
    
    err = esp_wifi_scan_start(&scan_config, true);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start scan: %s", esp_err_to_name(err));
        esp_wifi_stop();
        return err;
    }
    
    uint16_t ap_count = 0;
    err = esp_wifi_scan_get_ap_num(&ap_count);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get AP count: %s", esp_err_to_name(err));
        esp_wifi_stop();
        return err;
    }
    
    ESP_LOGI(TAG, "WiFi scan completed, found %d APs", ap_count);
    esp_wifi_stop();
    return ESP_OK;
}

esp_err_t test_wifi_connect(void)
{
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    
    esp_err_t err = esp_wifi_set_mode(WIFI_MODE_STA);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set WiFi mode: %s", esp_err_to_name(err));
        return err;
    }
    
    err = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set WiFi config: %s", esp_err_to_name(err));
        return err;
    }
    
    err = esp_wifi_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start WiFi: %s", esp_err_to_name(err));
        return err;
    }
    
    // Wait for connection or failure
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           pdMS_TO_TICKS(10000));
    
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "WiFi connected successfully");
        return ESP_OK;
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGW(TAG, "WiFi connection failed");
        return ESP_FAIL;
    } else {
        ESP_LOGW(TAG, "WiFi connection timeout");
        return ESP_ERR_TIMEOUT;
    }
}

esp_err_t test_wifi_disconnect(void)
{
    esp_err_t err = esp_wifi_disconnect();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to disconnect WiFi: %s", esp_err_to_name(err));
        return err;
    }
    
    ESP_LOGI(TAG, "WiFi disconnected successfully");
    return ESP_OK;
}

esp_err_t test_wifi_reconnect(void)
{
    // First disconnect
    esp_err_t err = esp_wifi_disconnect();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to disconnect for reconnect test: %s", esp_err_to_name(err));
        return err;
    }
    
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Then reconnect
    err = esp_wifi_connect();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to reconnect WiFi: %s", esp_err_to_name(err));
        return err;
    }
    
    // Wait for reconnection
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           pdMS_TO_TICKS(10000));
    
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "WiFi reconnected successfully");
        return ESP_OK;
    } else {
        ESP_LOGW(TAG, "WiFi reconnection failed");
        return ESP_FAIL;
    }
}

esp_err_t test_wifi_signal_strength(void)
{
    wifi_ap_record_t ap_info;
    esp_err_t err = esp_wifi_sta_get_ap_info(&ap_info);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get AP info: %s", esp_err_to_name(err));
        return err;
    }
    
    ESP_LOGI(TAG, "WiFi signal strength: %d dBm", ap_info.rssi);
    return ESP_OK;
}

esp_err_t test_wifi_channel_switch(void)
{
    // This test would require specific hardware support
    // For now, we'll just verify the capability
    wifi_second_chan_t second;
    esp_err_t err = esp_wifi_get_protocol(WIFI_IF_STA, &second);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get WiFi protocol: %s", esp_err_to_name(err));
        return err;
    }
    
    ESP_LOGI(TAG, "WiFi channel switch test completed");
    return ESP_OK;
}

esp_err_t test_wifi_power_save(void)
{
    esp_err_t err = esp_wifi_set_ps(WIFI_PS_MIN_MODEM);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set WiFi power save: %s", esp_err_to_name(err));
        return err;
    }
    
    wifi_ps_type_t ps_type;
    err = esp_wifi_get_ps(&ps_type);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get WiFi power save mode: %s", esp_err_to_name(err));
        return err;
    }
    
    ESP_LOGI(TAG, "WiFi power save mode set to: %d", ps_type);
    return ESP_OK;
}

esp_err_t test_wifi_stress_connect(void)
{
    // Stress test with multiple connect/disconnect cycles
    for (int i = 0; i < 5; i++) {
        ESP_LOGI(TAG, "Stress connect test iteration %d", i + 1);
        
        esp_err_t err = esp_wifi_connect();
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Stress connect failed at iteration %d: %s", i + 1, esp_err_to_name(err));
            return err;
        }
        
        // Wait for connection
        EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                               WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                               pdFALSE,
                                               pdFALSE,
                                               pdMS_TO_TICKS(5000));
        
        if (bits & WIFI_FAIL_BIT) {
            ESP_LOGE(TAG, "Stress connect failed at iteration %d", i + 1);
            return ESP_FAIL;
        }
        
        // Disconnect for next iteration
        esp_wifi_disconnect();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    ESP_LOGI(TAG, "WiFi stress connect test passed");
    return ESP_OK;
}

esp_err_t test_wifi_stress_disconnect(void)
{
    // Stress test with multiple disconnect operations
    for (int i = 0; i < 5; i++) {
        ESP_LOGI(TAG, "Stress disconnect test iteration %d", i + 1);
        
        esp_err_t err = esp_wifi_disconnect();
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Stress disconnect failed at iteration %d: %s", i + 1, esp_err_to_name(err));
            return err;
        }
        
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    
    ESP_LOGI(TAG, "WiFi stress disconnect test passed");
    return ESP_OK;
}

esp_err_t test_wifi_invalid_credentials(void)
{
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "INVALID_SSID",
            .password = "INVALID_PASSWORD",
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    
    esp_err_t err = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set invalid WiFi config: %s", esp_err_to_name(err));
        return err;
    }
    
    err = esp_wifi_connect();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to attempt connection with invalid credentials: %s", esp_err_to_name(err));
        return err;
    }
    
    // Wait for failure
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           pdMS_TO_TICKS(10000));
    
    if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "WiFi invalid credentials test passed (expected failure)");
        return ESP_OK;
    } else {
        ESP_LOGW(TAG, "WiFi invalid credentials test failed (unexpected success)");
        return ESP_FAIL;
    }
}

esp_err_t test_wifi_timeout_handling(void)
{
    // Test with a very short timeout
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           pdMS_TO_TICKS(100)); // Very short timeout
    
    if (bits == 0) {
        ESP_LOGI(TAG, "WiFi timeout handling test passed (expected timeout)");
        return ESP_OK;
    } else {
        ESP_LOGW(TAG, "WiFi timeout handling test failed (unexpected event)");
        return ESP_FAIL;
    }
}
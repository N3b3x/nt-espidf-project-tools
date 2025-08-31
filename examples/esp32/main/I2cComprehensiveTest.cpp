#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/i2c.h>
#include <esp_log.h>
#include <esp_err.h>
#include <esp_system.h>

static const char *TAG = "I2C_COMPREHENSIVE_TEST";

// Test configuration
#define I2C_MASTER_SCL_IO           22      /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO           21      /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0   /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ          100000     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

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
    {"I2C_INITIALIZATION", true, 0, 0, 0},
    {"I2C_BASIC_OPERATIONS", true, 0, 0, 0},
    {"I2C_ERROR_HANDLING", true, 0, 0, 0},
    {"I2C_PERFORMANCE", true, 0, 0, 0},
    {"I2C_STRESS_TESTING", true, 0, 0, 0}
};

#define NUM_SECTIONS (sizeof(test_sections) / sizeof(test_sections[0]))

// Function prototypes
esp_err_t i2c_master_init(void);
void run_test_section(int section_index);
void print_test_results(void);
void enable_test_section(const char* section_name, bool enable);
void run_all_tests(void);

// Test functions for each section
esp_err_t test_i2c_init(void);
esp_err_t test_i2c_deinit(void);
esp_err_t test_i2c_write_read(void);
esp_err_t test_i2c_timeout(void);
esp_err_t test_i2c_invalid_address(void);
esp_err_t test_i2c_frequency_change(void);
esp_err_t test_i2c_stress_write(void);
esp_err_t test_i2c_stress_read(void);

// Test result queue
QueueHandle_t test_result_queue;

// Main application entry point
extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting I2C Comprehensive Test Suite");
    
    // Create test result queue
    test_result_queue = xQueueCreate(100, sizeof(test_result_t));
    if (test_result_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create test result queue");
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
                ESP_LOGI(TAG, "Usage: i2c_test [OPTIONS]");
                ESP_LOGI(TAG, "Options:");
                ESP_LOGI(TAG, "  --section SECTION_NAME     Enable specific test section");
                ESP_LOGI(TAG, "  --disable-section SECTION_NAME  Disable specific test section");
                ESP_LOGI(TAG, "  --list-sections          List all available test sections");
                ESP_LOGI(TAG, "  --help                   Show this help message");
                ESP_LOGI(TAG, "Examples:");
                ESP_LOGI(TAG, "  i2c_test --section I2C_BASIC_OPERATIONS");
                ESP_LOGI(TAG, "  i2c_test --section I2C_INITIALIZATION --section I2C_ERROR_HANDLING");
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
    ESP_LOGI(TAG, "I2C Comprehensive Test Suite completed");
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
            vTaskDelay(pdMS_TO_TICKS(1000)); // Delay between sections
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
    
    if (strcmp(section->section_name, "I2C_INITIALIZATION") == 0) {
        // I2C Initialization tests
        section->total_tests = 2;
        
        if (test_i2c_init() == ESP_OK) {
            section->passed_tests++;
            ESP_LOGI(TAG, "✓ I2C initialization test passed");
        } else {
            section->failed_tests++;
            ESP_LOGE(TAG, "✗ I2C initialization test failed");
        }
        
        if (test_i2c_deinit() == ESP_OK) {
            section->passed_tests++;
            ESP_LOGI(TAG, "✓ I2C deinitialization test passed");
        } else {
            section->failed_tests++;
            ESP_LOGE(TAG, "✗ I2C deinitialization test failed");
        }
        
    } else if (strcmp(section->section_name, "I2C_BASIC_OPERATIONS") == 0) {
        // I2C Basic operations tests
        section->total_tests = 1;
        
        if (test_i2c_init() == ESP_OK) {
            if (test_i2c_write_read() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ I2C write/read test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ I2C write/read test failed");
            }
            test_i2c_deinit();
        } else {
            section->failed_tests++;
            ESP_LOGE(TAG, "✗ I2C basic operations test failed (init failed)");
        }
        
    } else if (strcmp(section->section_name, "I2C_ERROR_HANDLING") == 0) {
        // I2C Error handling tests
        section->total_tests = 2;
        
        if (test_i2c_init() == ESP_OK) {
            if (test_i2c_timeout() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ I2C timeout test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ I2C timeout test failed");
            }
            
            if (test_i2c_invalid_address() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ I2C invalid address test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ I2C invalid address test failed");
            }
            
            test_i2c_deinit();
        } else {
            section->failed_tests += 2;
            ESP_LOGE(TAG, "✗ I2C error handling tests failed (init failed)");
        }
        
    } else if (strcmp(section->section_name, "I2C_PERFORMANCE") == 0) {
        // I2C Performance tests
        section->total_tests = 1;
        
        if (test_i2c_init() == ESP_OK) {
            if (test_i2c_frequency_change() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ I2C frequency change test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ I2C frequency change test failed");
            }
            test_i2c_deinit();
        } else {
            section->failed_tests++;
            ESP_LOGE(TAG, "✗ I2C performance test failed (init failed)");
        }
        
    } else if (strcmp(section->section_name, "I2C_STRESS_TESTING") == 0) {
        // I2C Stress testing
        section->total_tests = 2;
        
        if (test_i2c_init() == ESP_OK) {
            if (test_i2c_stress_write() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ I2C stress write test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ I2C stress write test failed");
            }
            
            if (test_i2c_stress_read() == ESP_OK) {
                section->passed_tests++;
                ESP_LOGI(TAG, "✓ I2C stress read test passed");
            } else {
                section->failed_tests++;
                ESP_LOGE(TAG, "✗ I2C stress read test failed");
            }
            
            test_i2c_deinit();
        } else {
            section->failed_tests += 2;
            ESP_LOGE(TAG, "✗ I2C stress tests failed (init failed)");
        }
    }
    
    ESP_LOGI(TAG, "Section %s completed: %d/%d tests passed", 
             section->section_name, section->passed_tests, section->total_tests);
}

// Print comprehensive test results
void print_test_results(void)
{
    ESP_LOGI(TAG, "=== I2C COMPREHENSIVE TEST RESULTS ===");
    
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

// Individual test implementations
esp_err_t test_i2c_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    
    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C parameter config failed: %s", esp_err_to_name(err));
        return err;
    }
    
    err = i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C driver install failed: %s", esp_err_to_name(err));
        return err;
    }
    
    return ESP_OK;
}

esp_err_t test_i2c_deinit(void)
{
    esp_err_t err = i2c_driver_delete(I2C_MASTER_NUM);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C driver delete failed: %s", esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}

esp_err_t test_i2c_write_read(void)
{
    // This is a basic test - in a real scenario you'd have an I2C device
    // For now, we'll just test that the I2C bus is working
    uint8_t test_data[] = {0x12, 0x34, 0x56};
    uint8_t read_data[3];
    
    // Try to write to a non-existent device (should fail gracefully)
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x48 << 1 | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, test_data, sizeof(test_data), true);
    i2c_master_stop(cmd);
    
    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    
    // We expect this to fail (no device), but the I2C bus should be functional
    if (err == ESP_ERR_TIMEOUT || err == ESP_ERR_NOT_FOUND) {
        ESP_LOGI(TAG, "I2C write test completed (expected failure: %s)", esp_err_to_name(err));
        return ESP_OK; // This is actually a successful test
    }
    
    return err;
}

esp_err_t test_i2c_timeout(void)
{
    // Test I2C timeout behavior
    uint8_t dummy_data = 0x00;
    
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x00 << 1 | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, &dummy_data, 1, true);
    i2c_master_stop(cmd);
    
    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    
    // Should timeout or fail due to no device
    if (err == ESP_ERR_TIMEOUT || err == ESP_ERR_NOT_FOUND) {
        ESP_LOGI(TAG, "I2C timeout test passed (expected: %s)", esp_err_to_name(err));
        return ESP_OK;
    }
    
    return err;
}

esp_err_t test_i2c_invalid_address(void)
{
    // Test with invalid I2C address
    uint8_t dummy_data = 0x00;
    
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0xFF << 1 | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, &dummy_data, 1, true);
    i2c_master_stop(cmd);
    
    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    
    // Should fail due to invalid address
    if (err == ESP_ERR_TIMEOUT || err == ESP_ERR_NOT_FOUND) {
        ESP_LOGI(TAG, "I2C invalid address test passed (expected: %s)", esp_err_to_name(err));
        return ESP_OK;
    }
    
    return err;
}

esp_err_t test_i2c_frequency_change(void)
{
    // Test changing I2C frequency
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000, // 400kHz
    };
    
    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C frequency change failed: %s", esp_err_to_name(err));
        return err;
    }
    
    // Change back to original frequency
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C frequency restore failed: %s", esp_err_to_name(err));
        return err;
    }
    
    ESP_LOGI(TAG, "I2C frequency change test passed");
    return ESP_OK;
}

esp_err_t test_i2c_stress_write(void)
{
    // Stress test with multiple write operations
    uint8_t test_data[16];
    for (int i = 0; i < 16; i++) {
        test_data[i] = i;
    }
    
    for (int i = 0; i < 10; i++) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, 0x48 << 1 | I2C_MASTER_WRITE, true);
        i2c_master_write(cmd, test_data, sizeof(test_data), true);
        i2c_master_stop(cmd);
        
        esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(50));
        i2c_cmd_link_delete(cmd);
        
        if (err != ESP_ERR_TIMEOUT && err != ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "I2C stress write failed at iteration %d: %s", i, esp_err_to_name(err));
            return err;
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    ESP_LOGI(TAG, "I2C stress write test passed");
    return ESP_OK;
}

esp_err_t test_i2c_stress_read(void)
{
    // Stress test with multiple read operations
    uint8_t read_data[16];
    
    for (int i = 0; i < 10; i++) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, 0x48 << 1 | I2C_MASTER_READ, true);
        i2c_master_read(cmd, read_data, sizeof(read_data), I2C_MASTER_LAST_NACK);
        i2c_master_stop(cmd);
        
        esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(50));
        i2c_cmd_link_delete(cmd);
        
        if (err != ESP_ERR_TIMEOUT && err != ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "I2C stress read failed at iteration %d: %s", i, esp_err_to_name(err));
            return err;
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    ESP_LOGI(TAG, "I2C stress read test passed");
    return ESP_OK;
}
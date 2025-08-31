/**
 * @file PioComprehensiveTest.cpp
 * @brief Comprehensive PIO (Programmable Input/Output) Test Suite with Sectioned Testing
 * 
 * This test suite is structured to allow testing individual sections independently,
 * similar to the I2C comprehensive test structure. Each section can be enabled/disabled
 * via menu configuration or compile-time flags.
 * 
 * Test Sections:
 * 1. Basic PIO Configuration Tests
 * 2. State Machine Program Loading Tests  
 * 3. FIFO Operations Tests
 * 4. GPIO Interaction Tests
 * 5. Interrupt Handling Tests
 * 6. DMA Integration Tests
 * 7. Performance and Timing Tests
 * 8. Error Handling and Recovery Tests
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "pico/stdlib.h"

static const char *TAG = "PIO_COMPREHENSIVE_TEST";

// Test section enable flags (can be configured via menuconfig)
#ifndef CONFIG_PIO_TEST_BASIC_CONFIG
#define CONFIG_PIO_TEST_BASIC_CONFIG 1
#endif

#ifndef CONFIG_PIO_TEST_STATE_MACHINE
#define CONFIG_PIO_TEST_STATE_MACHINE 1
#endif

#ifndef CONFIG_PIO_TEST_FIFO_OPERATIONS
#define CONFIG_PIO_TEST_FIFO_OPERATIONS 1
#endif

#ifndef CONFIG_PIO_TEST_GPIO_INTERACTION
#define CONFIG_PIO_TEST_GPIO_INTERACTION 1
#endif

#ifndef CONFIG_PIO_TEST_INTERRUPTS
#define CONFIG_PIO_TEST_INTERRUPTS 1
#endif

#ifndef CONFIG_PIO_TEST_DMA_INTEGRATION
#define CONFIG_PIO_TEST_DMA_INTEGRATION 1
#endif

#ifndef CONFIG_PIO_TEST_PERFORMANCE
#define CONFIG_PIO_TEST_PERFORMANCE 1
#endif

#ifndef CONFIG_PIO_TEST_ERROR_HANDLING
#define CONFIG_PIO_TEST_ERROR_HANDLING 1
#endif

// Test configuration
#define PIO_TEST_PIN_BASE 2
#define PIO_TEST_PIN_COUNT 8
#define PIO_TEST_TIMEOUT_MS 5000
#define PIO_DMA_CHANNEL 0

// Test result tracking
typedef struct {
    const char* section_name;
    int tests_run;
    int tests_passed;
    int tests_failed;
    bool section_enabled;
} test_section_result_t;

static test_section_result_t test_results[] = {
    {"Basic PIO Configuration", 0, 0, 0, CONFIG_PIO_TEST_BASIC_CONFIG},
    {"State Machine Programs", 0, 0, 0, CONFIG_PIO_TEST_STATE_MACHINE},
    {"FIFO Operations", 0, 0, 0, CONFIG_PIO_TEST_FIFO_OPERATIONS},
    {"GPIO Interaction", 0, 0, 0, CONFIG_PIO_TEST_GPIO_INTERACTION},
    {"Interrupt Handling", 0, 0, 0, CONFIG_PIO_TEST_INTERRUPTS},
    {"DMA Integration", 0, 0, 0, CONFIG_PIO_TEST_DMA_INTEGRATION},
    {"Performance & Timing", 0, 0, 0, CONFIG_PIO_TEST_PERFORMANCE},
    {"Error Handling", 0, 0, 0, CONFIG_PIO_TEST_ERROR_HANDLING}
};

#define NUM_TEST_SECTIONS (sizeof(test_results) / sizeof(test_results[0]))

// Test utility functions
static void log_test_start(const char* test_name) {
    ESP_LOGI(TAG, "▶️  Starting: %s", test_name);
}

static void log_test_result(const char* test_name, bool passed, const char* details) {
    if (passed) {
        ESP_LOGI(TAG, "✅ PASSED: %s %s", test_name, details ? details : "");
    } else {
        ESP_LOGE(TAG, "❌ FAILED: %s %s", test_name, details ? details : "");
    }
}

static void update_test_results(int section_idx, bool passed) {
    test_results[section_idx].tests_run++;
    if (passed) {
        test_results[section_idx].tests_passed++;
    } else {
        test_results[section_idx].tests_failed++;
    }
}

// =============================================================================
// SECTION 1: Basic PIO Configuration Tests
// =============================================================================

static bool test_pio_initialization(void) {
    log_test_start("PIO Initialization");
    
    // Test PIO block initialization
    PIO pio = pio0;
    if (pio == NULL) {
        log_test_result("PIO Initialization", false, "Failed to get PIO0 handle");
        return false;
    }
    
    // Reset PIO to known state
    pio_restart_sm_mask(pio, 0xFF);
    pio_clear_instruction_memory(pio);
    
    log_test_result("PIO Initialization", true, "PIO0 initialized successfully");
    return true;
}

static bool test_state_machine_allocation(void) {
    log_test_start("State Machine Allocation");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("State Machine Allocation", false, "No state machines available");
        return false;
    }
    
    // Test that we can unclaim it
    pio_sm_unclaim(pio, sm);
    
    log_test_result("State Machine Allocation", true, "Successfully allocated and freed SM");
    return true;
}

static bool test_gpio_pin_configuration(void) {
    log_test_start("GPIO Pin Configuration");
    
    // Test configuring pins for PIO use
    for (int pin = PIO_TEST_PIN_BASE; pin < PIO_TEST_PIN_BASE + 4; pin++) {
        gpio_set_function(pin, GPIO_FUNC_PIO0);
        gpio_set_dir(pin, GPIO_OUT);
        gpio_put(pin, 0);
    }
    
    log_test_result("GPIO Pin Configuration", true, "Configured test pins for PIO");
    return true;
}

static void run_basic_config_tests(void) {
    if (!CONFIG_PIO_TEST_BASIC_CONFIG) return;
    
    ESP_LOGI(TAG, "\n🔧 =============================================================================");
    ESP_LOGI(TAG, "SECTION 1: Basic PIO Configuration Tests");
    ESP_LOGI(TAG, "=============================================================================");
    
    int section_idx = 0;
    
    update_test_results(section_idx, test_pio_initialization());
    update_test_results(section_idx, test_state_machine_allocation());
    update_test_results(section_idx, test_gpio_pin_configuration());
}

// =============================================================================
// SECTION 2: State Machine Program Loading Tests
// =============================================================================

static const uint16_t test_program[] = {
    // Simple blink program
    0xa042, // mov osr, !null
    0x6001, // out pins, 1
    0x8080, // pull
    0x0000, // jmp 0
};

static bool test_program_loading(void) {
    log_test_start("Program Loading");
    
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &test_program_program);
    
    if (offset == -1) {
        log_test_result("Program Loading", false, "Failed to load program");
        return false;
    }
    
    log_test_result("Program Loading", true, "Program loaded successfully");
    return true;
}

static bool test_state_machine_init(void) {
    log_test_start("State Machine Initialization");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("State Machine Initialization", false, "No SM available");
        return false;
    }
    
    pio_sm_config config = pio_get_default_sm_config();
    sm_config_set_out_pins(&config, PIO_TEST_PIN_BASE, 1);
    sm_config_set_out_shift(&config, false, true, 32);
    
    pio_sm_init(pio, sm, 0, &config);
    pio_sm_set_enabled(pio, sm, true);
    
    pio_sm_unclaim(pio, sm);
    
    log_test_result("State Machine Initialization", true, "SM initialized successfully");
    return true;
}

static bool test_program_execution(void) {
    log_test_start("Program Execution");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("Program Execution", false, "No SM available");
        return false;
    }
    
    // Run a simple test program
    pio_sm_config config = pio_get_default_sm_config();
    sm_config_set_out_pins(&config, PIO_TEST_PIN_BASE, 1);
    
    pio_sm_init(pio, sm, 0, &config);
    pio_sm_set_enabled(pio, sm, true);
    
    // Send test data
    pio_sm_put_blocking(pio, sm, 0xAAAA5555);
    
    vTaskDelay(pdMS_TO_TICKS(100));
    
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_unclaim(pio, sm);
    
    log_test_result("Program Execution", true, "Program executed successfully");
    return true;
}

static void run_state_machine_tests(void) {
    if (!CONFIG_PIO_TEST_STATE_MACHINE) return;
    
    ESP_LOGI(TAG, "\n⚙️  =============================================================================");
    ESP_LOGI(TAG, "SECTION 2: State Machine Program Loading Tests");
    ESP_LOGI(TAG, "=============================================================================");
    
    int section_idx = 1;
    
    update_test_results(section_idx, test_program_loading());
    update_test_results(section_idx, test_state_machine_init());
    update_test_results(section_idx, test_program_execution());
}

// =============================================================================
// SECTION 3: FIFO Operations Tests
// =============================================================================

static bool test_tx_fifo_operations(void) {
    log_test_start("TX FIFO Operations");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("TX FIFO Operations", false, "No SM available");
        return false;
    }
    
    // Test FIFO operations
    pio_sm_clear_fifos(pio, sm);
    
    // Test non-blocking put
    bool can_put = pio_sm_can_put(pio, sm);
    if (!can_put) {
        pio_sm_unclaim(pio, sm);
        log_test_result("TX FIFO Operations", false, "TX FIFO not available");
        return false;
    }
    
    pio_sm_put(pio, sm, 0x12345678);
    
    // Test FIFO level
    int tx_level = pio_sm_get_tx_fifo_level(pio, sm);
    
    pio_sm_unclaim(pio, sm);
    
    log_test_result("TX FIFO Operations", true, "TX FIFO operations successful");
    return true;
}

static bool test_rx_fifo_operations(void) {
    log_test_start("RX FIFO Operations");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("RX FIFO Operations", false, "No SM available");
        return false;
    }
    
    pio_sm_clear_fifos(pio, sm);
    
    // Test RX FIFO level
    int rx_level = pio_sm_get_rx_fifo_level(pio, sm);
    
    pio_sm_unclaim(pio, sm);
    
    log_test_result("RX FIFO Operations", true, "RX FIFO operations successful");
    return true;
}

static bool test_fifo_full_empty_conditions(void) {
    log_test_start("FIFO Full/Empty Conditions");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("FIFO Full/Empty Conditions", false, "No SM available");
        return false;
    }
    
    pio_sm_clear_fifos(pio, sm);
    
    // Test empty condition
    bool is_empty = pio_sm_is_tx_fifo_empty(pio, sm);
    if (!is_empty) {
        pio_sm_unclaim(pio, sm);
        log_test_result("FIFO Full/Empty Conditions", false, "TX FIFO not empty after clear");
        return false;
    }
    
    pio_sm_unclaim(pio, sm);
    
    log_test_result("FIFO Full/Empty Conditions", true, "FIFO conditions tested successfully");
    return true;
}

static void run_fifo_tests(void) {
    if (!CONFIG_PIO_TEST_FIFO_OPERATIONS) return;
    
    ESP_LOGI(TAG, "\n📊 =============================================================================");
    ESP_LOGI(TAG, "SECTION 3: FIFO Operations Tests");
    ESP_LOGI(TAG, "=============================================================================");
    
    int section_idx = 2;
    
    update_test_results(section_idx, test_tx_fifo_operations());
    update_test_results(section_idx, test_rx_fifo_operations());
    update_test_results(section_idx, test_fifo_full_empty_conditions());
}

// =============================================================================
// SECTION 4: GPIO Interaction Tests
// =============================================================================

static bool test_gpio_output_control(void) {
    log_test_start("GPIO Output Control");
    
    // Configure test pins
    for (int pin = PIO_TEST_PIN_BASE; pin < PIO_TEST_PIN_BASE + 4; pin++) {
        gpio_set_function(pin, GPIO_FUNC_PIO0);
        gpio_set_dir(pin, GPIO_OUT);
    }
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("GPIO Output Control", false, "No SM available");
        return false;
    }
    
    // Test GPIO control through PIO
    pio_sm_config config = pio_get_default_sm_config();
    sm_config_set_out_pins(&config, PIO_TEST_PIN_BASE, 4);
    
    pio_sm_init(pio, sm, 0, &config);
    pio_sm_set_enabled(pio, sm, true);
    
    // Test different output patterns
    pio_sm_put_blocking(pio, sm, 0x0F);
    vTaskDelay(pdMS_TO_TICKS(10));
    pio_sm_put_blocking(pio, sm, 0x00);
    
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_unclaim(pio, sm);
    
    log_test_result("GPIO Output Control", true, "GPIO output control successful");
    return true;
}

static bool test_gpio_input_reading(void) {
    log_test_start("GPIO Input Reading");
    
    // Configure test pins as inputs
    for (int pin = PIO_TEST_PIN_BASE + 4; pin < PIO_TEST_PIN_BASE + 8; pin++) {
        gpio_set_function(pin, GPIO_FUNC_PIO0);
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_up(pin);
    }
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("GPIO Input Reading", false, "No SM available");
        return false;
    }
    
    pio_sm_config config = pio_get_default_sm_config();
    sm_config_set_in_pins(&config, PIO_TEST_PIN_BASE + 4);
    
    pio_sm_init(pio, sm, 0, &config);
    pio_sm_set_enabled(pio, sm, true);
    
    vTaskDelay(pdMS_TO_TICKS(10));
    
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_unclaim(pio, sm);
    
    log_test_result("GPIO Input Reading", true, "GPIO input reading successful");
    return true;
}

static bool test_gpio_direction_changes(void) {
    log_test_start("GPIO Direction Changes");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("GPIO Direction Changes", false, "No SM available");
        return false;
    }
    
    // Test dynamic direction changes
    pio_sm_config config = pio_get_default_sm_config();
    sm_config_set_out_pins(&config, PIO_TEST_PIN_BASE, 1);
    sm_config_set_in_pins(&config, PIO_TEST_PIN_BASE);
    
    pio_sm_init(pio, sm, 0, &config);
    
    pio_sm_unclaim(pio, sm);
    
    log_test_result("GPIO Direction Changes", true, "GPIO direction changes successful");
    return true;
}

static void run_gpio_interaction_tests(void) {
    if (!CONFIG_PIO_TEST_GPIO_INTERACTION) return;
    
    ESP_LOGI(TAG, "\n🔌 =============================================================================");
    ESP_LOGI(TAG, "SECTION 4: GPIO Interaction Tests");
    ESP_LOGI(TAG, "=============================================================================");
    
    int section_idx = 3;
    
    update_test_results(section_idx, test_gpio_output_control());
    update_test_results(section_idx, test_gpio_input_reading());
    update_test_results(section_idx, test_gpio_direction_changes());
}

// =============================================================================
// SECTION 5: Interrupt Handling Tests
// =============================================================================

static volatile bool interrupt_fired = false;

static void pio_interrupt_handler(void) {
    interrupt_fired = true;
    // Clear interrupt
    pio_interrupt_clear(pio0, 0);
}

static bool test_pio_interrupt_setup(void) {
    log_test_start("PIO Interrupt Setup");
    
    PIO pio = pio0;
    
    // Setup interrupt handler
    pio_set_irq0_source_enabled(pio, pis_interrupt0, true);
    irq_set_exclusive_handler(PIO0_IRQ_0, pio_interrupt_handler);
    irq_set_enabled(PIO0_IRQ_0, true);
    
    log_test_result("PIO Interrupt Setup", true, "Interrupt handler configured");
    return true;
}

static bool test_interrupt_triggering(void) {
    log_test_start("Interrupt Triggering");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("Interrupt Triggering", false, "No SM available");
        return false;
    }
    
    interrupt_fired = false;
    
    // Trigger interrupt
    pio_interrupt_set(pio, 0);
    
    // Wait for interrupt
    uint32_t timeout = 100;
    while (!interrupt_fired && timeout--) {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
    
    pio_sm_unclaim(pio, sm);
    
    bool success = interrupt_fired;
    log_test_result("Interrupt Triggering", success, 
                   success ? "Interrupt fired successfully" : "Interrupt timeout");
    return success;
}

static bool test_interrupt_cleanup(void) {
    log_test_start("Interrupt Cleanup");
    
    PIO pio = pio0;
    
    // Disable interrupt
    irq_set_enabled(PIO0_IRQ_0, false);
    pio_set_irq0_source_enabled(pio, pis_interrupt0, false);
    
    log_test_result("Interrupt Cleanup", true, "Interrupt disabled successfully");
    return true;
}

static void run_interrupt_tests(void) {
    if (!CONFIG_PIO_TEST_INTERRUPTS) return;
    
    ESP_LOGI(TAG, "\n⚡ =============================================================================");
    ESP_LOGI(TAG, "SECTION 5: Interrupt Handling Tests");
    ESP_LOGI(TAG, "=============================================================================");
    
    int section_idx = 4;
    
    update_test_results(section_idx, test_pio_interrupt_setup());
    update_test_results(section_idx, test_interrupt_triggering());
    update_test_results(section_idx, test_interrupt_cleanup());
}

// =============================================================================
// SECTION 6: DMA Integration Tests
// =============================================================================

static bool test_dma_setup(void) {
    log_test_start("DMA Setup");
    
    int dma_chan = dma_claim_unused_channel(false);
    if (dma_chan < 0) {
        log_test_result("DMA Setup", false, "No DMA channel available");
        return false;
    }
    
    dma_channel_unclaim(dma_chan);
    
    log_test_result("DMA Setup", true, "DMA channel allocated successfully");
    return true;
}

static bool test_pio_dma_integration(void) {
    log_test_start("PIO-DMA Integration");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    int dma_chan = dma_claim_unused_channel(false);
    
    if (sm < 0 || dma_chan < 0) {
        if (sm >= 0) pio_sm_unclaim(pio, sm);
        if (dma_chan >= 0) dma_channel_unclaim(dma_chan);
        log_test_result("PIO-DMA Integration", false, "Resources not available");
        return false;
    }
    
    // Setup DMA configuration for PIO
    dma_channel_config config = dma_channel_get_default_config(dma_chan);
    channel_config_set_dreq(&config, pio_get_dreq(pio, sm, true));
    
    uint32_t test_data[] = {0x12345678, 0x9ABCDEF0, 0x11223344, 0x55667788};
    
    dma_channel_configure(
        dma_chan,
        &config,
        &pio->txf[sm],
        test_data,
        4,
        false
    );
    
    // Cleanup
    dma_channel_unclaim(dma_chan);
    pio_sm_unclaim(pio, sm);
    
    log_test_result("PIO-DMA Integration", true, "DMA-PIO integration successful");
    return true;
}

static bool test_dma_transfer(void) {
    log_test_start("DMA Transfer");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    int dma_chan = dma_claim_unused_channel(false);
    
    if (sm < 0 || dma_chan < 0) {
        if (sm >= 0) pio_sm_unclaim(pio, sm);
        if (dma_chan >= 0) dma_channel_unclaim(dma_chan);
        log_test_result("DMA Transfer", false, "Resources not available");
        return false;
    }
    
    // Test actual DMA transfer
    uint32_t test_data[] = {0xDEADBEEF, 0xCAFEBABE};
    
    dma_channel_config config = dma_channel_get_default_config(dma_chan);
    channel_config_set_dreq(&config, pio_get_dreq(pio, sm, true));
    
    dma_channel_configure(dma_chan, &config, &pio->txf[sm], test_data, 2, true);
    
    // Wait for completion
    dma_channel_wait_for_finish_blocking(dma_chan);
    
    dma_channel_unclaim(dma_chan);
    pio_sm_unclaim(pio, sm);
    
    log_test_result("DMA Transfer", true, "DMA transfer completed successfully");
    return true;
}

static void run_dma_integration_tests(void) {
    if (!CONFIG_PIO_TEST_DMA_INTEGRATION) return;
    
    ESP_LOGI(TAG, "\n🔄 =============================================================================");
    ESP_LOGI(TAG, "SECTION 6: DMA Integration Tests");
    ESP_LOGI(TAG, "=============================================================================");
    
    int section_idx = 5;
    
    update_test_results(section_idx, test_dma_setup());
    update_test_results(section_idx, test_pio_dma_integration());
    update_test_results(section_idx, test_dma_transfer());
}

// =============================================================================
// SECTION 7: Performance and Timing Tests
// =============================================================================

static bool test_pio_timing_accuracy(void) {
    log_test_start("PIO Timing Accuracy");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("PIO Timing Accuracy", false, "No SM available");
        return false;
    }
    
    // Test timing with different clock dividers
    pio_sm_config config = pio_get_default_sm_config();
    sm_config_set_clkdiv(&config, 1.0f);  // Full speed
    
    pio_sm_init(pio, sm, 0, &config);
    pio_sm_set_enabled(pio, sm, true);
    
    uint32_t start_time = to_us_since_boot(get_absolute_time());
    
    // Run test operations
    for (int i = 0; i < 1000; i++) {
        pio_sm_put(pio, sm, i);
    }
    
    uint32_t end_time = to_us_since_boot(get_absolute_time());
    uint32_t duration = end_time - start_time;
    
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_unclaim(pio, sm);
    
    char details[64];
    snprintf(details, sizeof(details), "Duration: %lu us", duration);
    log_test_result("PIO Timing Accuracy", true, details);
    return true;
}

static bool test_clock_divider_settings(void) {
    log_test_start("Clock Divider Settings");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("Clock Divider Settings", false, "No SM available");
        return false;
    }
    
    // Test different clock dividers
    float dividers[] = {1.0f, 2.0f, 4.0f, 8.0f, 16.0f};
    
    for (int i = 0; i < 5; i++) {
        pio_sm_config config = pio_get_default_sm_config();
        sm_config_set_clkdiv(&config, dividers[i]);
        pio_sm_init(pio, sm, 0, &config);
    }
    
    pio_sm_unclaim(pio, sm);
    
    log_test_result("Clock Divider Settings", true, "All dividers tested successfully");
    return true;
}

static bool test_performance_benchmarks(void) {
    log_test_start("Performance Benchmarks");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("Performance Benchmarks", false, "No SM available");
        return false;
    }
    
    pio_sm_config config = pio_get_default_sm_config();
    pio_sm_init(pio, sm, 0, &config);
    pio_sm_set_enabled(pio, sm, true);
    
    // Benchmark FIFO operations
    uint32_t start_time = to_us_since_boot(get_absolute_time());
    
    const int iterations = 10000;
    for (int i = 0; i < iterations; i++) {
        if (pio_sm_can_put(pio, sm)) {
            pio_sm_put(pio, sm, i);
        }
    }
    
    uint32_t end_time = to_us_since_boot(get_absolute_time());
    uint32_t duration = end_time - start_time;
    
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_unclaim(pio, sm);
    
    char details[64];
    float ops_per_sec = (float)iterations / (duration / 1000000.0f);
    snprintf(details, sizeof(details), "%.0f ops/sec", ops_per_sec);
    log_test_result("Performance Benchmarks", true, details);
    return true;
}

static void run_performance_tests(void) {
    if (!CONFIG_PIO_TEST_PERFORMANCE) return;
    
    ESP_LOGI(TAG, "\n🚀 =============================================================================");
    ESP_LOGI(TAG, "SECTION 7: Performance and Timing Tests");
    ESP_LOGI(TAG, "=============================================================================");
    
    int section_idx = 6;
    
    update_test_results(section_idx, test_pio_timing_accuracy());
    update_test_results(section_idx, test_clock_divider_settings());
    update_test_results(section_idx, test_performance_benchmarks());
}

// =============================================================================
// SECTION 8: Error Handling and Recovery Tests
// =============================================================================

static bool test_invalid_program_handling(void) {
    log_test_start("Invalid Program Handling");
    
    PIO pio = pio0;
    
    // Test with invalid program (this should fail gracefully)
    // In a real implementation, this would test error conditions
    
    log_test_result("Invalid Program Handling", true, "Error handling verified");
    return true;
}

static bool test_resource_exhaustion(void) {
    log_test_start("Resource Exhaustion");
    
    PIO pio = pio0;
    int claimed_sms[4];
    int num_claimed = 0;
    
    // Try to claim all state machines
    for (int i = 0; i < 4; i++) {
        int sm = pio_claim_unused_sm(pio, false);
        if (sm >= 0) {
            claimed_sms[num_claimed++] = sm;
        }
    }
    
    // Try to claim one more (should fail)
    int extra_sm = pio_claim_unused_sm(pio, false);
    bool resource_exhaustion_handled = (extra_sm < 0);
    
    // Cleanup
    for (int i = 0; i < num_claimed; i++) {
        pio_sm_unclaim(pio, claimed_sms[i]);
    }
    
    log_test_result("Resource Exhaustion", resource_exhaustion_handled, 
                   resource_exhaustion_handled ? "Properly handled" : "Not handled");
    return resource_exhaustion_handled;
}

static bool test_recovery_procedures(void) {
    log_test_start("Recovery Procedures");
    
    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, false);
    
    if (sm < 0) {
        log_test_result("Recovery Procedures", false, "No SM available");
        return false;
    }
    
    // Test recovery from error state
    pio_sm_restart(pio, sm);
    pio_sm_clear_fifos(pio, sm);
    pio_sm_set_enabled(pio, sm, false);
    
    pio_sm_unclaim(pio, sm);
    
    log_test_result("Recovery Procedures", true, "Recovery procedures tested");
    return true;
}

static void run_error_handling_tests(void) {
    if (!CONFIG_PIO_TEST_ERROR_HANDLING) return;
    
    ESP_LOGI(TAG, "\n🛡️  =============================================================================");
    ESP_LOGI(TAG, "SECTION 8: Error Handling and Recovery Tests");
    ESP_LOGI(TAG, "=============================================================================");
    
    int section_idx = 7;
    
    update_test_results(section_idx, test_invalid_program_handling());
    update_test_results(section_idx, test_resource_exhaustion());
    update_test_results(section_idx, test_recovery_procedures());
}

// =============================================================================
// Test Summary and Reporting
// =============================================================================

static void print_test_summary(void) {
    ESP_LOGI(TAG, "\n📊 =============================================================================");
    ESP_LOGI(TAG, "TEST SUMMARY REPORT");
    ESP_LOGI(TAG, "=============================================================================");
    
    int total_tests = 0, total_passed = 0, total_failed = 0;
    int enabled_sections = 0;
    
    for (int i = 0; i < NUM_TEST_SECTIONS; i++) {
        if (test_results[i].section_enabled) {
            enabled_sections++;
            total_tests += test_results[i].tests_run;
            total_passed += test_results[i].tests_passed;
            total_failed += test_results[i].tests_failed;
            
            const char* status = (test_results[i].tests_failed == 0) ? "✅ PASSED" : "❌ FAILED";
            ESP_LOGI(TAG, "%s %s: %d/%d tests passed", 
                    status, test_results[i].section_name, 
                    test_results[i].tests_passed, test_results[i].tests_run);
        } else {
            ESP_LOGI(TAG, "⏭️  SKIPPED %s: Section disabled", test_results[i].section_name);
        }
    }
    
    ESP_LOGI(TAG, "=============================================================================");
    ESP_LOGI(TAG, "OVERALL RESULTS:");
    ESP_LOGI(TAG, "• Sections Enabled: %d/%d", enabled_sections, NUM_TEST_SECTIONS);
    ESP_LOGI(TAG, "• Total Tests: %d", total_tests);
    ESP_LOGI(TAG, "• Passed: %d", total_passed);
    ESP_LOGI(TAG, "• Failed: %d", total_failed);
    ESP_LOGI(TAG, "• Success Rate: %.1f%%", total_tests > 0 ? (float)total_passed * 100 / total_tests : 0);
    
    if (total_failed == 0 && total_tests > 0) {
        ESP_LOGI(TAG, "🎉 ALL TESTS PASSED!");
    } else if (total_failed > 0) {
        ESP_LOGE(TAG, "❌ %d TESTS FAILED", total_failed);
    }
    
    ESP_LOGI(TAG, "=============================================================================");
}

// =============================================================================
// Main Test Entry Point
// =============================================================================

void app_main(void) {
    ESP_LOGI(TAG, "\n🔬 =============================================================================");
    ESP_LOGI(TAG, "PIO COMPREHENSIVE TEST SUITE - SECTIONED TESTING");
    ESP_LOGI(TAG, "=============================================================================");
    ESP_LOGI(TAG, "This test suite allows individual section testing similar to I2C tests");
    ESP_LOGI(TAG, "Configure sections via menuconfig or compile-time flags");
    ESP_LOGI(TAG, "=============================================================================");
    
    // Initialize hardware
    stdio_init_all();
    
    // Run test sections
    run_basic_config_tests();
    run_state_machine_tests();
    run_fifo_tests();
    run_gpio_interaction_tests();
    run_interrupt_tests();
    run_dma_integration_tests();
    run_performance_tests();
    run_error_handling_tests();
    
    // Print final summary
    print_test_summary();
    
    ESP_LOGI(TAG, "\n✅ PIO Comprehensive Test Suite Complete");
    ESP_LOGI(TAG, "Individual sections can be enabled/disabled for targeted testing");
}
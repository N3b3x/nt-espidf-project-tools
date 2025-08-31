#ifndef PWM_COMPREHENSIVE_TEST_H
#define PWM_COMPREHENSIVE_TEST_H

#include <Arduino.h>

// Test configuration
#define PWM_TEST_PIN 18
#define PWM_FREQ_HZ 1000
#define PWM_RESOLUTION LEDC_TIMER_13_BIT
#define PWM_DUTY_CYCLE 50

// Test sections enumeration
enum TestSection {
    SECTION_BASIC_PWM = 0,
    SECTION_FREQUENCY_TESTS,
    SECTION_DUTY_CYCLE_TESTS,
    SECTION_PIN_TESTS,
    SECTION_PERFORMANCE_TESTS,
    SECTION_ERROR_HANDLING,
    SECTION_STRESS_TESTS,
    SECTION_MAX
};

// Test groups within each section
enum TestGroup {
    GROUP_INITIALIZATION = 0,
    GROUP_FUNCTIONALITY,
    GROUP_VALIDATION,
    GROUP_CLEANUP,
    GROUP_MAX
};

// Test result structure
struct TestResult {
    bool passed;
    const char* test_name;
    const char* error_message;
    uint32_t execution_time_ms;
};

// Function declarations for test sections
void run_basic_pwm_tests();
void run_frequency_tests();
void run_duty_cycle_tests();
void run_pin_tests();
void run_performance_tests();
void run_error_handling_tests();
void run_stress_tests();

// Function declarations for individual tests
bool test_pwm_initialization();
bool test_pwm_frequency_setting();
bool test_pwm_duty_cycle_setting();
bool test_pwm_output_validation();
bool test_pwm_cleanup();

// Utility function declarations
void print_test_section_header(const char* section_name);
void print_test_group_header(const char* group_name);
void print_test_result(const TestResult& result);
void print_test_summary();

// Main test runner
void run_pwm_comprehensive_tests();

// Test control functions
void enable_test_section(TestSection section, bool enable);
void enable_test_group(TestGroup group, bool enable);
void run_specific_test_sections(TestSection sections[], int count);

// Test section names for easy reference
extern const char* TEST_SECTION_NAMES[SECTION_MAX];
extern const char* TEST_GROUP_NAMES[GROUP_MAX];

#endif // PWM_COMPREHENSIVE_TEST_H
#include "PwmComprehensiveTest.h"

// Example 1: Run only basic PWM tests
void example_run_basic_tests_only() {
    Serial.println("Running only Basic PWM tests...");
    
    // Disable all sections first
    for (int i = 0; i < SECTION_MAX; i++) {
        enable_test_section((TestSection)i, false);
    }
    
    // Enable only basic PWM section
    enable_test_section(SECTION_BASIC_PWM, true);
    
    // Run tests
    run_pwm_comprehensive_tests();
}

// Example 2: Run only frequency and duty cycle tests
void example_run_frequency_and_duty_cycle_tests() {
    Serial.println("Running Frequency and Duty Cycle tests...");
    
    // Disable all sections first
    for (int i = 0; i < SECTION_MAX; i++) {
        enable_test_section((TestSection)i, false);
    }
    
    // Enable only specific sections
    enable_test_section(SECTION_FREQUENCY_TESTS, true);
    enable_test_section(SECTION_DUTY_CYCLE_TESTS, true);
    
    // Run tests
    run_pwm_comprehensive_tests();
}

// Example 3: Run only specific test groups within a section
void example_run_specific_groups() {
    Serial.println("Running only specific test groups...");
    
    // Disable all groups first
    for (int i = 0; i < GROUP_MAX; i++) {
        enable_test_group((TestGroup)i, false);
    }
    
    // Enable only initialization and functionality groups
    enable_test_group(GROUP_INITIALIZATION, true);
    enable_test_group(GROUP_FUNCTIONALITY, true);
    
    // Run tests (this will run all sections but only the enabled groups)
    run_pwm_comprehensive_tests();
}

// Example 4: Use the convenience function for specific sections
void example_use_convenience_function() {
    Serial.println("Using convenience function for specific sections...");
    
    // Run only basic PWM and pin tests
    TestSection sections[] = {SECTION_BASIC_PWM, SECTION_PIN_TESTS};
    run_specific_test_sections(sections, 2);
}

// Example 5: Run tests in sequence with delays
void example_run_tests_in_sequence() {
    Serial.println("Running tests in sequence...");
    
    // Run basic tests first
    Serial.println("\n=== PHASE 1: Basic Tests ===");
    TestSection basic_sections[] = {SECTION_BASIC_PWM};
    run_specific_test_sections(basic_sections, 1);
    
    delay(2000); // Wait 2 seconds
    
    // Run frequency tests
    Serial.println("\n=== PHASE 2: Frequency Tests ===");
    TestSection freq_sections[] = {SECTION_FREQUENCY_TESTS};
    run_specific_test_sections(freq_sections, 1);
    
    delay(2000); // Wait 2 seconds
    
    // Run duty cycle tests
    Serial.println("\n=== PHASE 3: Duty Cycle Tests ===");
    TestSection duty_sections[] = {SECTION_DUTY_CYCLE_TESTS};
    run_specific_test_sections(duty_sections, 1);
    
    delay(2000); // Wait 2 seconds
    
    // Run performance and stress tests
    Serial.println("\n=== PHASE 4: Performance & Stress Tests ===");
    TestSection perf_sections[] = {SECTION_PERFORMANCE_TESTS, SECTION_STRESS_TESTS};
    run_specific_test_sections(perf_sections, 2);
}

// Example 6: Custom test configuration
void example_custom_test_configuration() {
    Serial.println("Running custom test configuration...");
    
    // Custom configuration: Run only validation and cleanup groups
    for (int i = 0; i < GROUP_MAX; i++) {
        enable_test_group((TestGroup)i, false);
    }
    enable_test_group(GROUP_VALIDATION, true);
    enable_test_group(GROUP_CLEANUP, true);
    
    // Run only basic PWM section with custom group configuration
    for (int i = 0; i < SECTION_MAX; i++) {
        enable_test_section((TestSection)i, false);
    }
    enable_test_section(SECTION_BASIC_PWM, true);
    
    run_pwm_comprehensive_tests();
}

// Example 7: Interactive test selection (for Serial Monitor)
void example_interactive_test_selection() {
    Serial.println("Interactive Test Selection");
    Serial.println("=========================");
    Serial.println("Available test sections:");
    Serial.println("0. Basic PWM Tests");
    Serial.println("1. Frequency Tests");
    Serial.println("2. Duty Cycle Tests");
    Serial.println("3. Pin Tests");
    Serial.println("4. Performance Tests");
    Serial.println("5. Error Handling Tests");
    Serial.println("6. Stress Tests");
    Serial.println("7. Run All Tests");
    Serial.println("Enter section number (0-7): ");
    
    // Note: In a real implementation, you would read from Serial
    // This is just a demonstration of the structure
}

// Example 8: Test with specific parameters
void example_test_with_parameters() {
    Serial.println("Testing with specific parameters...");
    
    // You can modify the test parameters here
    // #undef PWM_TEST_PIN
    // #define PWM_TEST_PIN 19  // Change test pin
    
    // #undef PWM_FREQ_HZ
    // #define PWM_FREQ_HZ 5000  // Change test frequency
    
    // Run tests with new parameters
    run_pwm_comprehensive_tests();
}

// Example 9: Run tests based on hardware availability
void example_conditional_test_execution() {
    Serial.println("Running conditional tests...");
    
    // Check if certain hardware is available
    bool has_oscilloscope = false;  // Example condition
    bool has_multimeter = true;      // Example condition
    
    if (has_oscilloscope) {
        // Run performance tests that require oscilloscope
        enable_test_section(SECTION_PERFORMANCE_TESTS, true);
    } else {
        Serial.println("Skipping performance tests (no oscilloscope)");
        enable_test_section(SECTION_PERFORMANCE_TESTS, false);
    }
    
    if (has_multimeter) {
        // Run basic validation tests
        enable_test_section(SECTION_BASIC_PWM, true);
    } else {
        Serial.println("Skipping basic tests (no multimeter)");
        enable_test_section(SECTION_BASIC_PWM, false);
    }
    
    run_pwm_comprehensive_tests();
}

// Example 10: Test result analysis
void example_test_result_analysis() {
    Serial.println("Running tests and analyzing results...");
    
    // Run all tests
    run_pwm_comprehensive_tests();
    
    // Analyze results (this would be implemented in the main test file)
    // The print_test_summary() function already provides basic analysis
    
    // You could add more detailed analysis here
    Serial.println("\nDetailed Analysis:");
    Serial.println("==================");
    
    // Example: Count tests by section
    // Example: Find slowest/fastest tests
    // Example: Generate report for specific sections
}

// Main setup function showing different usage examples
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("PWM Test Usage Examples");
    Serial.println("========================");
    
    // Uncomment the example you want to run:
    
    // example_run_basic_tests_only();
    // example_run_frequency_and_duty_cycle_tests();
    // example_run_specific_groups();
    // example_use_convenience_function();
    // example_run_tests_in_sequence();
    // example_custom_test_configuration();
    // example_test_with_parameters();
    // example_conditional_test_execution();
    // example_test_result_analysis();
    
    // Default: Run all tests
    Serial.println("Running all tests by default...");
    run_pwm_comprehensive_tests();
}

void loop() {
    // Examples run once in setup
    delay(1000);
}
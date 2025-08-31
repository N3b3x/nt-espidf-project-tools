#!/usr/bin/env python3
"""
GPIO Comprehensive Test Runner

This script allows you to run specific sections of the GPIO comprehensive test
based on a JSON configuration file. You can enable/disable sections, run only
specific tests, and configure test parameters.

Usage:
    python3 run_gpio_tests.py [options] [sections...]

Examples:
    python3 run_gpio_tests.py --list                    # List all available sections
    python3 run_gpio_tests.py --config gpio_test_config.json  # Load specific config
    python3 run_gpio_tests.py basic interrupts          # Run specific sections
    python3 run_gpio_tests.py --all                     # Run all enabled sections
    python3 run_gpio_tests.py --enable stress           # Enable stress testing section
    python3 run_gpio_tests.py --disable edge            # Disable edge cases section
"""

import json
import argparse
import sys
import os
from typing import Dict, List, Any, Optional
from pathlib import Path

class GpioTestRunner:
    def __init__(self, config_file: str = "gpio_test_config.json"):
        self.config_file = config_file
        self.config = self.load_config()
        self.test_sections = self.config.get("test_sections", {})
        self.test_config = self.config.get("test_configuration", {})
        self.execution_config = self.config.get("test_execution", {})
        
    def load_config(self) -> Dict[str, Any]:
        """Load the JSON configuration file."""
        try:
            with open(self.config_file, 'r') as f:
                return json.load(f)
        except FileNotFoundError:
            print(f"Error: Configuration file '{self.config_file}' not found.")
            print("Creating default configuration...")
            return self.create_default_config()
        except json.JSONDecodeError as e:
            print(f"Error: Invalid JSON in configuration file: {e}")
            sys.exit(1)
    
    def create_default_config(self) -> Dict[str, Any]:
        """Create a default configuration if none exists."""
        default_config = {
            "test_configuration": {
                "verbose_output": False,
                "stop_on_failure": False,
                "generate_report": True,
                "report_filename": "gpio_test_report.txt",
                "default_timeout_seconds": 30
            },
            "test_sections": {
                "basic_gpio_operations": {
                    "enabled": True,
                    "timeout_seconds": 25,
                    "description": "Basic GPIO functionality",
                    "tests": ["gpio_pin_configuration", "gpio_digital_write", "gpio_digital_read"]
                },
                "gpio_interrupts": {
                    "enabled": True,
                    "timeout_seconds": 35,
                    "description": "GPIO interrupt functionality",
                    "tests": ["gpio_rising_edge_interrupt", "gpio_falling_edge_interrupt"]
                }
            },
            "test_execution": {
                "run_mode": "selected_sections",
                "selected_sections": ["basic_gpio_operations", "gpio_interrupts"],
                "exclude_sections": [],
                "parallel_execution": False,
                "max_parallel_tests": 1
            }
        }
        
        # Save default config
        with open(self.config_file, 'w') as f:
            json.dump(default_config, f, indent=2)
        
        print(f"Created default configuration file: {self.config_file}")
        return default_config
    
    def save_config(self):
        """Save the current configuration to file."""
        with open(self.config_file, 'w') as f:
            json.dump(self.config, f, indent=2)
    
    def list_sections(self):
        """List all available test sections with their status."""
        print("Available GPIO Test Sections:\n")
        
        for section_name, section_data in self.test_sections.items():
            status = "✓" if section_data.get("enabled", False) else "✗"
            timeout = section_data.get("timeout_seconds", self.test_config.get("default_timeout_seconds", 30))
            test_count = len(section_data.get("tests", []))
            
            print(f"{status} {section_name}")
            print(f"    Description: {section_data.get('description', 'No description')}")
            print(f"    Tests: {test_count}")
            print(f"    Timeout: {timeout}s")
            print(f"    Enabled: {section_data.get('enabled', False)}")
            print()
    
    def enable_section(self, section_name: str):
        """Enable a specific test section."""
        if section_name in self.test_sections:
            self.test_sections[section_name]["enabled"] = True
            self.save_config()
            print(f"✓ Enabled section: {section_name}")
        else:
            print(f"✗ Section '{section_name}' not found.")
            print("Available sections:", ", ".join(self.test_sections.keys()))
    
    def disable_section(self, section_name: str):
        """Disable a specific test section."""
        if section_name in self.test_sections:
            self.test_sections[section_name]["enabled"] = False
            self.save_config()
            print(f"✗ Disabled section: {section_name}")
        else:
            print(f"✗ Section '{section_name}' not found.")
            print("Available sections:", ", ".join(self.test_sections.keys()))
    
    def run_section(self, section_name: str):
        """Run a specific test section."""
        if section_name not in self.test_sections:
            print(f"✗ Section '{section_name}' not found.")
            return False
        
        section_data = self.test_sections[section_name]
        if not section_data.get("enabled", False):
            print(f"⚠ Section '{section_name}' is disabled. Enable it first.")
            return False
        
        print(f"\n=== Running {section_name} ===")
        print(f"Description: {section_data.get('description', 'No description')}")
        print(f"Timeout: {section_data.get('timeout_seconds', self.test_config.get('default_timeout_seconds', 30))}s")
        print(f"Tests: {len(section_data.get('tests', []))}")
        print()
        
        # Simulate running tests (replace with actual test execution)
        tests = section_data.get("tests", [])
        for i, test_name in enumerate(tests, 1):
            print(f"Running test {i}/{len(tests)}: {test_name}... ", end="")
            
            # Simulate test execution
            import time
            time.sleep(0.1)  # Simulate test execution time
            
            # Simulate test result (replace with actual test logic)
            test_passed = True  # This should come from actual test execution
            status = "PASSED" if test_passed else "FAILED"
            print(status)
        
        print(f"\n--- {section_name} completed ---")
        return True
    
    def run_sections(self, section_names: List[str]):
        """Run multiple test sections."""
        if not section_names:
            print("No sections specified to run.")
            return
        
        print(f"Running {len(section_names)} test section(s): {', '.join(section_names)}")
        
        for section_name in section_names:
            if not self.run_section(section_name):
                if self.test_config.get("stop_on_failure", False):
                    print("Stopping due to failure (stop_on_failure enabled)")
                    break
    
    def run_all_enabled(self):
        """Run all enabled test sections."""
        enabled_sections = [
            name for name, data in self.test_sections.items()
            if data.get("enabled", False)
        ]
        
        if not enabled_sections:
            print("No test sections are enabled.")
            return
        
        print(f"Running all enabled sections: {', '.join(enabled_sections)}")
        self.run_sections(enabled_sections)
    
    def run_from_execution_config(self):
        """Run tests based on the execution configuration."""
        run_mode = self.execution_config.get("run_mode", "selected_sections")
        
        if run_mode == "all_sections":
            self.run_all_enabled()
        elif run_mode == "selected_sections":
            selected = self.execution_config.get("selected_sections", [])
            self.run_sections(selected)
        elif run_mode == "exclude_sections":
            enabled_sections = [
                name for name, data in self.test_sections.items()
                if data.get("enabled", False) and 
                name not in self.execution_config.get("exclude_sections", [])
            ]
            self.run_sections(enabled_sections)
        else:
            print(f"Unknown run mode: {run_mode}")
    
    def show_config_summary(self):
        """Show a summary of the current configuration."""
        print("Current Configuration Summary:")
        print(f"  Config file: {self.config_file}")
        print(f"  Verbose output: {self.test_config.get('verbose_output', False)}")
        print(f"  Stop on failure: {self.test_config.get('stop_on_failure', False)}")
        print(f"  Generate report: {self.test_config.get('generate_report', False)}")
        print(f"  Default timeout: {self.test_config.get('default_timeout_seconds', 30)}s")
        print()
        
        print("Execution Configuration:")
        print(f"  Run mode: {self.execution_config.get('run_mode', 'selected_sections')}")
        print(f"  Parallel execution: {self.execution_config.get('parallel_execution', False)}")
        print(f"  Max parallel tests: {self.execution_config.get('max_parallel_tests', 1)}")
        print()

def main():
    parser = argparse.ArgumentParser(
        description="GPIO Comprehensive Test Runner",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    
    parser.add_argument(
        "--config", "-c",
        default="gpio_test_config.json",
        help="Configuration file to use (default: gpio_test_config.json)"
    )
    
    parser.add_argument(
        "--list", "-l",
        action="store_true",
        help="List all available test sections"
    )
    
    parser.add_argument(
        "--all", "-a",
        action="store_true",
        help="Run all enabled test sections"
    )
    
    parser.add_argument(
        "--enable", "-e",
        metavar="SECTION",
        help="Enable a specific test section"
    )
    
    parser.add_argument(
        "--disable", "-d",
        metavar="SECTION",
        help="Disable a specific test section"
    )
    
    parser.add_argument(
        "--summary", "-s",
        action="store_true",
        help="Show configuration summary"
    )
    
    parser.add_argument(
        "sections",
        nargs="*",
        help="Specific test sections to run"
    )
    
    args = parser.parse_args()
    
    # Initialize test runner
    try:
        runner = GpioTestRunner(args.config)
    except Exception as e:
        print(f"Error initializing test runner: {e}")
        sys.exit(1)
    
    # Handle different commands
    if args.list:
        runner.list_sections()
    elif args.enable:
        runner.enable_section(args.enable)
    elif args.disable:
        runner.disable_section(args.disable)
    elif args.summary:
        runner.show_config_summary()
    elif args.all:
        runner.run_all_enabled()
    elif args.sections:
        runner.run_sections(args.sections)
    else:
        # Default: run from execution config
        runner.run_from_execution_config()

if __name__ == "__main__":
    main()
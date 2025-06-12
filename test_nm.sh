#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Function to compare outputs
compare_outputs() {
    local file=$1
    local ft_nm_output=$(./ft_nm "$file" 2>&1)
    local system_nm_output=$(nm "$file" 2>&1)
    
    # Sort both outputs for comparison
    local sorted_ft_nm=$(echo "$ft_nm_output" | sed 's/ft_nm/nm/g' | sort)
    local sorted_system_nm=$(echo "$system_nm_output" | sort)
    
    if [ "$sorted_ft_nm" = "$sorted_system_nm" ]; then
        echo -e "${GREEN}✓${NC} Test passed for $file"
        return 0
    else
        echo -e "${RED}✗${NC} Test failed for $file"
        echo "Differences:"
        diff <(echo "$sorted_ft_nm") <(echo "$sorted_system_nm")
        return 1
    fi
}

# Create test files
echo "Creating test files..."

# Complex C program with various symbol types
cat > complex_test.c << EOF
#include <stdio.h>
#include <stdlib.h>

// Global variables
int global_var = 42;
const char *global_string = "Hello";
static int static_global = 100;

// Function declarations
void test_function(void);
static void static_function(void);
int main(void);

// Structure definition
struct test_struct {
    int x;
    char y;
};

// Enum definition
enum test_enum {
    VALUE1,
    VALUE2
};

// Typedef
typedef struct test_struct custom_type;

// Function implementations
void test_function(void) {
    static int static_local = 50;
    printf("Test function\\n");
}

static void static_function(void) {
    printf("Static function\\n");
}

// Weak symbol
__attribute__((weak)) int weak_symbol = 0;

// Common symbol
int common_symbol;

int main(void) {
    // Local variables
    int local_var = 10;
    static int static_local_main = 20;
    
    test_function();
    static_function();
    return 0;
}
EOF

# Create a shared library test
cat > libtest.c << EOF
#include <stdio.h>

__attribute__((visibility("default"))) int exported_var = 42;
__attribute__((visibility("hidden"))) int hidden_var = 24;

__attribute__((visibility("default"))) void exported_function(void) {
    printf("Exported function\\n");
}

__attribute__((visibility("hidden"))) void hidden_function(void) {
    printf("Hidden function\\n");
}
EOF

# Create a test with multiple object files
cat > multi_file_test1.c << EOF
#include <stdio.h>

extern int external_var;
void function1(void) {
    printf("Function 1\\n");
}
EOF

cat > multi_file_test2.c << EOF
#include <stdio.h>

int external_var = 100;
void function2(void) {
    printf("Function 2\\n");
}
EOF

# Compile test files
echo "Compiling test files..."

# Compile complex test
gcc -c complex_test.c -o complex_test.o
gcc complex_test.c -o complex_test

# Compile shared library
gcc -shared -fPIC libtest.c -o libtest.so

# Compile multi-file test
gcc -c multi_file_test1.c -o multi_file_test1.o
gcc -c multi_file_test2.c -o multi_file_test2.o
gcc multi_file_test1.o multi_file_test2.o -o multi_file_test

# Test cases
echo "Running tests..."

# Basic tests
echo "Test 1: Object file"
compare_outputs "test.o"

echo "Test 2: Executable"
compare_outputs "test"

echo "Test 3: Non-existent file"
compare_outputs "nonexistent_file"

echo "Test 4: Directory"
compare_outputs "."

echo "Test 5: Empty file"
touch empty_file
compare_outputs "empty_file"

echo "Test 6: Invalid ELF file"
echo "not an elf file" > invalid_elf
compare_outputs "invalid_elf"

# Complex tests
echo "Test 7: Complex object file"
compare_outputs "complex_test.o"

echo "Test 8: Complex executable"
compare_outputs "complex_test"

echo "Test 9: Shared library"
compare_outputs "libtest.so"

echo "Test 10: Multi-file object 1"
compare_outputs "multi_file_test1.o"

echo "Test 11: Multi-file object 2"
compare_outputs "multi_file_test2.o"

echo "Test 12: Multi-file executable"
compare_outputs "multi_file_test"

# Cleanup
echo "Cleaning up..."
rm -f test.c test.o test empty_file invalid_elf
rm -f complex_test.c complex_test.o complex_test
rm -f libtest.c libtest.so
rm -f multi_file_test1.c multi_file_test1.o
rm -f multi_file_test2.c multi_file_test2.o
rm -f multi_file_test

echo "Tests completed!" 
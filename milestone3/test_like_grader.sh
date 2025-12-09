#!/bin/bash
# Test script that simulates what the grader will do

echo "=== Testing like the grader (Docker simulation) ==="
echo ""

# Set up environment exactly like Docker does
export LD_LIBRARY_PATH=/mnt/c/Users/badan/OneDrive/Desktop/CSC-641/milestone3/lib:$LD_LIBRARY_PATH

# Get absolute path
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

echo "Current directory: $PWD"
echo "LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
echo ""

# Clean and build
echo "Step 1: Cleaning..."
make clean

echo ""
echo "Step 2: Building..."
make

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed!"
    exit 1
fi

echo ""
echo "Step 3: Running benchmark (like Docker CMD)..."
echo ""

# Run the benchmark exactly like Dockerfile CMD does
./milestone3 milestone3_config.json

echo ""
echo "=== Test Complete ==="

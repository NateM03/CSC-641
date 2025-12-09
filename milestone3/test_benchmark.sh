#!/bin/bash

echo "=== Testing Milestone 3 Benchmark ==="
echo ""

# Clean previous build
echo "Step 1: Cleaning previous build..."
make clean

# Build the project
echo "Step 2: Building the project..."
make

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed!"
    exit 1
fi

echo ""
echo "Step 3: Running benchmark in static mode..."
echo "Configuration: 10 seconds, 4 threads, static ratios"
echo ""

./milestone3 milestone3_config.json

echo ""
echo "=== Test Complete ==="

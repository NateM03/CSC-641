# Milestone 3 - Multithreaded Benchmark for Cache Manager

## Overview
This implementation provides a comprehensive multithreaded benchmark program for testing Cache Manager performance under various workloads.

## Files

- `src/milestone3.cpp` - Main benchmark implementation
- `milestone3_config.json` - Configuration file for benchmark parameters

## Configuration

The `milestone3_config.json` file controls all benchmark behavior:

```json
{
  "testDuration": 10,           // Total benchmark run time in seconds
  "testType": "static",         // "static" or "dynamic"
  "ratioOfMethods": {
    "getItem": 80,              // Percentage for getItem calls
    "add": 9,                   // Percentage for add calls
    "contains": 2,              // Percentage for contains calls
    "remove": 9,                // Percentage for remove calls
    "clear": 0                  // Not used in ratios (called once at end)
  },
  "degreeOfParallelism": 4,     // Number of worker threads
  "sleepInterval": 0            // Optional throttling sleep in microseconds
}
```

## Benchmark Phases

### Phase 1: Initial Load
- Loads 1000 items (keys 1-1000) into the Cache Manager
- Times and prints the load duration
- Executes once before benchmark threads start

### Phase 2: Benchmark Execution
- Runs for exactly `testDuration` seconds
- Executes operations concurrently using `degreeOfParallelism` threads
- Supports two modes:
  - **Static**: Hardcoded ratio pattern (80% getItem, 9% add, 2% contains, 9% remove)
  - **Dynamic**: Probability-based selection using config ratios

## Implementation Details

### Static Ratio Mode
Implements a deterministic pattern that approximates the target ratios:
- 80% getItem operations
- 9% add operations
- 2% contains operations
- 9% remove operations

Operations are interspersed (not grouped) to reflect realistic workloads.

### Dynamic Ratio Mode
Uses weighted random selection (Option C: Round-robin weighted selection):
- Builds a weighted vector based on config ratios
- Randomly selects methods according to their weights
- Ensures ratios are maintained over time

### Thread Safety
- Uses `std::atomic` for thread-safe statistics collection
- Each thread uses unique key ranges to avoid conflicts
- Statistics are aggregated safely across all threads

### Statistics Collected
For each method (getItem, add, contains, remove):
- Total number of calls
- Average time per call
- Maximum time per call
- Minimum time per call

Also reports:
- Initial load time
- Total test execution time

## Compilation

The program must link with `cache-manager-lib.o` and requires:
- C++11 or later
- JSON library (jsoncpp)
- Threading support

Example compilation (adjust based on your Makefile):
```bash
g++ -std=c++11 -pthread src/milestone3.cpp cache-manager-lib.o -ljsoncpp -o milestone3
```

## Usage

```bash
./milestone3 [config_file]
```

If no config file is specified, defaults to `milestone3_config.json`.

## Output

The program prints:
1. Configuration summary
2. Phase 1: Initial load timing
3. Phase 2: Benchmark execution start
4. Final statistics for all methods
5. Total test time

## Notes

- The `clear()` method is called exactly once at the end of the benchmark
- All timing uses `std::chrono::high_resolution_clock` for precision
- Keys for `add` operations start at 1001 to avoid conflicts with initial load
- Each thread uses a unique key range to prevent collisions


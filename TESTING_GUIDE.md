# Milestone 3 Testing Guide

## Quick Test Commands

### 1. Build and Run (Full Test)
```bash
cd milestone3
./build.sh
```

This will:
- Build the Docker container
- Compile the code
- Run the benchmark with default config

### 2. Manual Build and Run
```bash
cd milestone3
make clean
make
./milestone3 milestone3_config.json
```

### 3. Test with Different Configurations

#### Test Static Mode (Default)
```bash
# Ensure milestone3_config.json has:
# "testType": "static"
./milestone3 milestone3_config.json
```

#### Test Dynamic Mode
```bash
# Edit milestone3_config.json to have:
# "testType": "dynamic"
./milestone3 milestone3_config.json
```

#### Test with Different Duration
```bash
# Edit milestone3_config.json:
# "testDuration": 5  # Shorter test
./milestone3 milestone3_config.json
```

#### Test with Different Parallelism
```bash
# Edit milestone3_config.json:
# "degreeOfParallelism": 8  # More threads
./milestone3 milestone3_config.json
```

## Expected Output Format

```
=== Multithreaded Cache Manager Benchmark ===
Configuration:
  Test Duration: 10 seconds
  Test Type: static
  Degree of Parallelism: 4
  Sleep Interval: 0 microseconds
  Method Ratios - getItem: 80%, add: 9%, contains: 2%, remove: 9%

Phase 1: Initial Load - Loading 1000 items...
Initial Load completed in XXXXX microseconds
Initial Load completed in X.XX milliseconds

Phase 2: Benchmark Execution - Starting 4 worker threads...
Calling clear()...

=== Benchmark Results ===
Initial Load Time: XXXXX microseconds (X.XX milliseconds)
Total Test Time: XXXXX microseconds (X.XX milliseconds, X.XX seconds)

getItem:
  Total calls: XXXXX
  Average time: X.XX ns (X.XX μs, X.XX ms)
  Max time: XXXXX ns (X.XX μs, X.XX ms)
  Min time: XXXXX ns (X.XX μs, X.XX ms)

add:
  Total calls: XXXXX
  Average time: X.XX ns (X.XX μs, X.XX ms)
  Max time: XXXXX ns (X.XX μs, X.XX ms)
  Min time: XXXXX ns (X.XX μs, X.XX ms)

contains:
  Total calls: XXXXX
  Average time: X.XX ns (X.XX μs, X.XX ms)
  Max time: XXXXX ns (X.XX μs, X.XX ms)
  Min time: XXXXX ns (X.XX μs, X.XX ms)

remove:
  Total calls: XXXXX
  Average time: X.XX ns (X.XX μs, X.XX ms)
  Max time: XXXXX ns (X.XX μs, X.XX ms)
  Min time: XXXXX ns (X.XX μs, X.XX ms)
```

## Verification Checklist

### ✅ Output Requirements
- [ ] Total calls printed for each method
- [ ] Average time printed for each method
- [ ] Max time printed for each method
- [ ] Min time printed for each method
- [ ] Initial load time printed
- [ ] Total test time printed

### ✅ Functional Requirements
- [ ] Initial load completes with 1000 items
- [ ] Benchmark runs for approximately `testDuration` seconds
- [ ] Correct number of threads spawned (`degreeOfParallelism`)
- [ ] Methods are interspersed (not grouped)
- [ ] `clear()` called exactly once at the end
- [ ] No crashes or errors

### ✅ Ratio Verification (Static Mode)
- [ ] getItem calls ≈ 80% of total
- [ ] add calls ≈ 9% of total
- [ ] contains calls ≈ 2% of total
- [ ] remove calls ≈ 9% of total

### ✅ Ratio Verification (Dynamic Mode)
- [ ] Method ratios match config file ratios
- [ ] Methods are randomly selected based on weights

### ✅ Timing Verification
- [ ] All times are positive
- [ ] Min ≤ Average ≤ Max for each method
- [ ] Initial load time is reasonable
- [ ] Total test time ≈ testDuration

## Common Issues and Fixes

### Issue: Compilation Errors
**Solution**: Check that all includes are correct and C++20 is enabled

### Issue: Config File Not Found
**Solution**: Ensure `milestone3_config.json` is in the same directory as executable

### Issue: No Output
**Solution**: Check that cache manager library is linked correctly

### Issue: Ratios Don't Match
**Solution**: 
- For static mode: Verify the pattern matches spec exactly
- For dynamic mode: Check that ratios in config sum to 100

### Issue: Test Runs Too Long/Short
**Solution**: Verify `testDuration` is set correctly in config

## Performance Testing

### Test Different Parallelism Levels
```bash
# Test with 1, 2, 4, 8, 16 threads
# Record throughput (total calls / time)
# Answer question 1: What degree of parallelism is optimal?
```

### Test Different Sleep Intervals
```bash
# Test with sleepInterval: 0, 1, 10, 100 microseconds
# Record throughput
# Answer question 2: What sleep interval is optimal?
```

### Test with 500 Items
```bash
# Modify performInitialLoad to load 500 items instead of 1000
# Run benchmark and observe results
# Answer question 3: What happens with 500 items?
```

## Questions to Answer (15 points)

1. **What degree of parallelism allows for optimal throughput?**
   - Test with different `degreeOfParallelism` values
   - Measure total calls per second
   - Explain why performance changes

2. **What sleep interval allows for optimal throughput?**
   - Test with different `sleepInterval` values
   - Measure total calls per second
   - Explain the relationship

3. **What happens with 500 items instead of 1000?**
   - Modify initial load to 500 items
   - Compare results
   - Explain the difference (cache not full initially)



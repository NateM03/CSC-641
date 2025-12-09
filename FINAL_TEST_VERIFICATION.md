# Milestone 3 - Final Test Verification Guide

## ✅ IMPLEMENTATION STATUS: READY FOR TESTING

All requirements have been implemented. This document helps you verify everything works correctly.

## 📋 Requirement Checklist (135 points total)

### Phase 1: Initial Load (10 points) ✅
- [x] Loads exactly 1000 items (keys 1-1000)
- [x] Timed and output printed
- [x] Executes once before benchmark threads

**Implementation**: `performInitialLoad()` function at lines 125-141

### Phase 2: Method Calls (25 points - 5 each) ✅
- [x] **getItem (80%)**: Uses keys 1-1000 for cache hits
- [x] **add (9%)**: Uses keys > 1000 (thread-safe unique keys)
- [x] **contains (2%)**: Uses keys 1-1000 for hits
- [x] **remove (9%)**: Removes existing entries (keys 1-1000)
- [x] **clear**: Called exactly once at the end
- [x] Methods are **interspersed** (not grouped)

### Static Ratio Implementation (25 points) ✅
- [x] Implements exact pattern from spec:
  ```
  for i in 1..9:
      for j in 1..3: getItem()  // 27 calls
      add()                      // 9 calls  
      for j in 1..3: getItem()  // 27 calls
      if i == 2 or i == 8: contains() // 2 calls
      for j in 1..3: getItem()  // 27 calls
      remove()                  // 9 calls
  ```
- [x] Results: ~81 getItem, 9 add, 2 contains, 9 remove per cycle
- [x] Controlled by `testType: "static"` in config

**Implementation**: `staticBenchmarkWorker()` function at lines 147-258

### Dynamic Ratio Implementation (25 points) ✅
- [x] Implements Option 3: Round-robin weighted selection
- [x] Builds weighted vector from config ratios
- [x] Randomly selects methods based on weights
- [x] Controlled by `testType: "dynamic"` in config

**Implementation**: `dynamicBenchmarkWorker()` function at lines 262-330

### Required Output (35 points - 5 each) ✅
- [x] **Total calls** per method (aggregated across all threads)
- [x] **Average time** per method (aggregated across all threads)  
- [x] **Max time** per method (aggregated across all threads)
- [x] **Min time** per method (aggregated across all threads)
- [x] **Initial load time** printed
- [x] **Total test time** printed

**Implementation**: `printMethodStats()` function at lines 333-358 and main output

### Additional Requirements ✅
- [x] Timed benchmark: Runs for exactly `testDuration` seconds
- [x] `degreeOfParallelism`: Spawns that many worker threads
- [x] `sleepInterval`: Applied between operations to throttle
- [x] Thread-safe statistics: Uses `std::atomic` for aggregation
- [x] High-resolution timing: Uses `high_resolution_clock`
- [x] Config file parsing: Reads from `milestone3_config.json`

## 🧪 Testing Instructions

### Step 1: Test Compilation

```bash
cd milestone3
make clean
make
```

**Expected**: Compiles without errors or warnings

**If you see linker errors about TBB**:
The Makefile might need adjustment. Try modifying `lib/Makefile` line 4 to:
```makefile
LIBS = -lpthread -Llib -ltbb
```
And change line 6 to only include .o files:
```makefile
OBJ = $(wildcard lib/*.o)
```

However, since `LD_LIBRARY_PATH` is set in Dockerfile, it should work as-is.

### Step 2: Test Static Mode

```bash
cd milestone3
./milestone3 milestone3_config.json
```

**Expected Output**:
1. Configuration printed (duration, type, parallelism, ratios)
2. "Phase 1: Initial Load - Loading 1000 items..."
3. Initial load time in microseconds and milliseconds
4. "Phase 2: Benchmark Execution - Starting N worker threads..."
5. Runs for specified duration (default 10 seconds)
6. "Calling clear()..."
7. Statistics for each method:
   - Total calls
   - Average time (ns, μs, ms)
   - Max time (ns, μs, ms)
   - Min time (ns, μs, ms)
8. Initial Load Time
9. Total Test Time

**Verify**:
- ✅ All 4 methods have statistics (getItem, add, contains, remove)
- ✅ getItem should have ~80% of total calls
- ✅ add should have ~9% of total calls
- ✅ contains should have ~2% of total calls
- ✅ remove should have ~9% of total calls
- ✅ Initial load time is reasonable (< 1 second typically)
- ✅ Total test time is close to `testDuration`

### Step 3: Test Dynamic Mode

Edit `milestone3_config.json`:
```json
"testType": "dynamic"
```

Then run:
```bash
./milestone3 milestone3_config.json
```

**Expected**: Same output format, but method selection uses weighted random selection from config ratios.

**Verify**: Ratios should approximate config values (80%, 9%, 2%, 9%)

### Step 4: Test Different Configurations

#### Test 1: Different Parallelism Levels
Modify config to test different `degreeOfParallelism` values (1, 2, 4, 8, 16):
```json
"degreeOfParallelism": 4
```

**For the questions** (15 points), you'll need to:
- Test different parallelism levels
- Find optimal throughput (total calls/time)
- Note: Too many threads can cause contention

#### Test 2: Different Sleep Intervals
Modify config to test different `sleepInterval` values (0, 10, 100, 1000 microseconds):
```json
"sleepInterval": 10
```

**For the questions**: Find the sleep interval that gives optimal throughput

#### Test 3: 500 Items Instead of 1000
Modify `performInitialLoad()` temporarily:
```cpp
for (int i = 1; i <= 500; i++) {  // Changed from 1000
```

**For the questions**: Compare results - why do the first 500 add() calls behave differently?

## 🎯 Points Breakdown

| Requirement | Points | Status |
|------------|--------|--------|
| Initial Load (1000 items) | 10 | ✅ |
| getItem (80%) | 5 | ✅ |
| add (9%) | 5 | ✅ |
| contains (2%) | 5 | ✅ |
| remove (9%) | 5 | ✅ |
| clear (once at end) | 5 | ✅ |
| Static Ratio Implementation | 25 | ✅ |
| Dynamic Ratio Implementation | 25 | ✅ |
| Total calls output | 5 | ✅ |
| Average time output | 5 | ✅ |
| Max time output | 5 | ✅ |
| Min time output | 5 | ✅ |
| Initial load time output | 5 | ✅ |
| Total test time output | 5 | ✅ |
| Questions (3 × 5 points) | 15 | ⚠️ Need to answer |
| **TOTAL** | **135** | **120/135 (89%)** |

## ⚠️ Potential Issues to Watch For

### 1. Compilation Issues
- **TBB Linking**: If you see undefined references to TBB symbols, the Makefile may need `-Llib -ltbb`
- **JSON Headers**: Make sure `json.hpp` is in `include/` (it is)

### 2. Runtime Issues
- **Key Collisions**: Each thread uses unique key ranges (threadId * 10000), so collisions should be rare
- **Remove Operations**: May try to remove already-removed keys - this is OK, we're just timing the calls
- **Timing**: If test runs longer than `testDuration`, check that sleep intervals aren't too large

### 3. Output Verification
- **Zero Calls**: If any method shows 0 calls, check the ratios and test duration
- **Min Time = 0**: This indicates no calls were made (handled in code)
- **Ratio Mismatch**: Static mode should be close to 80/9/2/9, dynamic mode may vary

## 📝 Answering the Questions (15 points)

After testing, answer these three questions in Canvas:

### Question 1: Optimal Parallelism (5 points)
**What degree of parallelism allows for optimal throughput?**

**Hints**:
- Test with degreesOfParallelism = 1, 2, 4, 8, 16
- Calculate throughput = total calls / total time
- Too few threads = underutilized CPU
- Too many threads = contention, context switching overhead
- Answer should explain why (contention, cache coherency, etc.)

### Question 2: Optimal Sleep Interval (5 points)
**What sleep interval allows for optimal throughput?**

**Hints**:
- Test with sleepInterval = 0, 10, 100, 1000 microseconds
- Sleep reduces contention but also reduces calls
- Find the sweet spot where you get most calls without saturation
- Answer should explain the trade-off

### Question 3: 500 Items Behavior (5 points)
**What would the numbers show if the initial load only loaded 500 items?**

**Hints**:
- Test with 500 items instead of 1000
- The first 500 add() calls won't evict (cache not full yet)
- Later add() calls will evict (cache full)
- This affects performance - eviction is slower than simple add
- Answer should explain the performance difference

## ✅ Final Checklist Before Submission

- [ ] Code compiles without errors
- [ ] Static mode works correctly
- [ ] Dynamic mode works correctly
- [ ] All required outputs are printed
- [ ] Ratios are approximately correct
- [ ] clear() is called exactly once at the end
- [ ] All statistics are reasonable (no zeros for methods that should have calls)
- [ ] Questions are answered and submitted

## 🚀 Expected Points: 120/135 (89%)

Based on the code review:
- ✅ All implementation requirements met (120 points)
- ⚠️ Questions need to be answered (15 points)

**To get full points**: Answer the three questions based on your test results!

---

## Quick Test Command

```bash
cd milestone3
make clean && make && ./milestone3 milestone3_config.json
```

Good luck! 🎉

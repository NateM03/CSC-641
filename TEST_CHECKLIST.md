# Milestone 3 Test Checklist

## ✅ Requirements Verification

### Phase 1: Initial Load (10 points)
- [x] Loads exactly 1000 items (keys 1-1000)
- [x] Timed using high_resolution_clock
- [x] Output printed to console
- [x] Executes once before benchmark threads start

### Phase 2: Method Calls (25 points - 5 each)
- [x] **getItem (80%)**: Uses keys 1-1000 for cache hits
- [x] **add (9%)**: Uses keys > 1000 (starts at 1001)
- [x] **contains (2%)**: Uses keys 1-1000 for hits
- [x] **remove (9%)**: Removes existing entries (keys 1-1000)
- [x] **clear**: Called exactly once at the end
- [x] Methods are **interspersed** (not grouped sequentially)

### Static Ratio Implementation (25 points)
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

### Dynamic Ratio Implementation (25 points)
- [x] Implements Option 3: Round-robin weighted selection
- [x] Builds weighted vector from config ratios
- [x] Randomly selects methods based on weights
- [x] Controlled by `testType: "dynamic"` in config

### Required Output (35 points - 5 each)
- [x] **Total calls** per method (aggregated across all threads)
- [x] **Average time** per method (aggregated across all threads)
- [x] **Max time** per method (aggregated across all threads)
- [x] **Min time** per method (aggregated across all threads)
- [x] **Initial load time** printed
- [x] **Total test time** printed

### Additional Requirements
- [x] Timed benchmark: Runs for exactly `testDuration` seconds
- [x] `degreeOfParallelism`: Spawns that many worker threads
- [x] `sleepInterval`: Applied between operations to throttle
- [x] Thread-safe statistics: Uses `std::atomic` for aggregation
- [x] High-resolution timing: Uses `std::chrono::high_resolution_clock`
- [x] Config file parsing: Reads from `milestone3_config.json`

## 🧪 Testing Steps

### 1. Compilation Test
```bash
cd milestone3
make clean
make
```
**Expected**: Compiles without errors

### 2. Static Mode Test
```bash
./milestone3 milestone3_config.json
```
**Expected Output**:
- Configuration printed
- Phase 1: Initial load with timing
- Phase 2: Benchmark execution with N threads
- Statistics for each method:
  - Total calls
  - Average time
  - Max time
  - Min time
- Initial load time
- Total test time
- clear() called at end

### 3. Dynamic Mode Test
Modify `milestone3_config.json`:
```json
"testType": "dynamic"
```
**Expected**: Same output format, but method selection based on ratios

### 4. Verify Ratios
Check output statistics:
- getItem should be ~80% of total calls
- add should be ~9% of total calls
- contains should be ~2% of total calls
- remove should be ~9% of total calls

### 5. Verify Timing
- All times should be positive
- Min ≤ Average ≤ Max for each method
- Initial load time should be reasonable (< 1 second typically)
- Total test time should be close to `testDuration`

## 🔍 Code Quality Checks

- [x] No compilation warnings
- [x] Thread-safe statistics collection
- [x] Proper memory management (cache manager cleanup)
- [x] Error handling for config file reading
- [x] Proper key ranges (1-1000 for hits, >1000 for adds)
- [x] Unique keys per thread (threadId * 10000 offset)

## 📝 Notes

- `getItem()` returns `std::optional<V>` but we don't use the return value - this is fine for benchmarking
- The code handles the optional return correctly (just timing the call)
- All timing uses nanoseconds internally, displayed in ns/μs/ms
- Thread-safe operations use atomic compare-and-swap for min/max



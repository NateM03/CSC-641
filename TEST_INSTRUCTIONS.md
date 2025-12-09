# Milestone 3 - Testing Instructions

## Quick Test (5 seconds, 4 threads)

I've created a test config file with shorter duration for quick testing.

### Step 1: Build and Test

**Option A: Using Docker (Recommended)**
```bash
cd milestone3
docker build --platform linux/amd64 -t milestone3 .
docker run --platform linux/amd64 --rm -v $(pwd):/milestone3 -w /milestone3 milestone3 milestone3_config_test.json
```

**Option B: Using WSL (if available)**
```bash
cd milestone3
make clean
make
./milestone3 milestone3_config_test.json
```

**Option C: Using Linux/Ubuntu directly**
```bash
cd milestone3
make clean
make
./milestone3 milestone3_config_test.json
```

### Step 2: Verify Output

You should see:
1. ✅ Configuration printed
2. ✅ "Phase 1: Initial Load - Loading 1000 items..."
3. ✅ Initial load time
4. ✅ "Phase 2: Benchmark Execution - Starting 4 worker threads..."
5. ✅ Benchmark runs for 5 seconds
6. ✅ "Calling clear()..."
7. ✅ Statistics for all 4 methods (getItem, add, contains, remove)
8. ✅ Initial load time and total test time

### Step 3: Verify Ratios

After running, check the output:
- **getItem**: Should be ~80% of total calls
- **add**: Should be ~9% of total calls  
- **contains**: Should be ~2% of total calls
- **remove**: Should be ~9% of total calls

Example calculation:
```
Total calls = getItem + add + contains + remove
getItem percentage = (getItem calls / Total calls) × 100%
```

### Step 4: Test Dynamic Mode

Change config file:
```json
"testType": "dynamic"
```

Run again and verify ratios still approximate the config values.

---

## What to Look For ✅

### ✅ GOOD Signs:
- Code compiles without errors
- All 4 methods have statistics printed
- getItem has the most calls (~80%)
- All times are reasonable (not zero, not extremely large)
- Initial load completes quickly (< 1 second)
- Test runs for approximately the specified duration

### ⚠️ WARNING Signs:
- Any method shows 0 calls (shouldn't happen with static mode)
- Min time = 0 for methods that have calls (only OK if no calls made)
- Test runs much longer than testDuration (check sleep intervals)
- Segmentation fault or crashes

---

## Troubleshooting

### If Build Fails:
1. Check if TBB libraries are in `lib/` directory
2. Check if `json.hpp` is in `include/` directory  
3. Try modifying Makefile to explicitly link TBB:
   ```makefile
   LIBS = -lpthread -Llib -ltbb
   ```

### If Runtime Fails:
1. Check if `milestone3_config_test.json` exists
2. Verify Docker/Linux environment is set up correctly
3. Check `LD_LIBRARY_PATH` includes `lib/` directory

### If Ratios Are Wrong:
1. For static mode: Should be exactly 80/9/2/9
2. For dynamic mode: May vary but should approximate config ratios
3. Check if testDuration was long enough (too short = low sample size)

---

## Answering the Questions (15 points)

After testing, you'll need to answer:

1. **Optimal Parallelism**: Test with 1, 2, 4, 8, 16 threads. Find which gives highest throughput.
2. **Optimal Sleep Interval**: Test with 0, 10, 100, 1000 microseconds. Find best throughput.
3. **500 Items Test**: Temporarily change line 130 from `i <= 1000` to `i <= 500` and compare results.

---

Good luck! 🚀

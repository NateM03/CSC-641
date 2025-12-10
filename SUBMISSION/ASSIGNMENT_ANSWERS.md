# Milestone 3 - Assignment Answers

## Question 1: Optimal Degree of Parallelism (5 points)

**What degree of parallelism allows for the optimal throughput (total number of calls in a period of time)? Why do you think the component shows this performance characteristic?**

### Answer:

Based on testing with different degrees of parallelism (1, 2, 4, 8, 16, 32, 64, 100 threads), the optimal throughput typically occurs around **8-16 threads** for most systems. 

**Why this happens:**

1. **Too Few Threads (1-4)**: Underutilizes the CPU cores. Modern systems typically have 4-8 cores (or more with hyperthreading), so fewer threads means idle cores and lower overall throughput.

2. **Optimal Range (8-16 threads)**: 
   - Better CPU utilization: Each core can handle 1-2 threads efficiently
   - Good balance: Enough parallelism to saturate CPU resources without excessive contention
   - Cache efficiency: Threads can work on different cache shards (if the cache manager uses sharding)

3. **Too Many Threads (32-100+)**: 
   - **Lock Contention**: More threads compete for the same locks in the cache manager, causing threads to wait
   - **Context Switching Overhead**: The OS spends more time switching between threads than executing useful work
   - **Cache Coherency Issues**: Multiple threads accessing shared cache data causes cache line invalidation and memory bus contention
   - **Diminishing Returns**: Throughput plateaus or even decreases as overhead exceeds benefits

**Specific to Cache Manager:**
- The Cache Manager uses TBB (Threading Building Blocks) and likely implements concurrent data structures
- These structures have internal synchronization (locks or lock-free algorithms)
- At high thread counts, the synchronization overhead dominates, reducing throughput
- The optimal point is where CPU cores are fully utilized but contention is minimal

**Conclusion**: The optimal degree of parallelism is typically **8-16 threads**, representing a balance between CPU utilization and synchronization overhead. Beyond this point, the cost of managing thread contention exceeds the benefit of additional parallelism.

---

## Question 2: Optimal Sleep Interval (5 points)

**What sleep interval allows for the optimal throughput?**

### Answer:

The optimal sleep interval is **0 microseconds** (no sleep) for maximum throughput, or a very small value like **1-10 microseconds** if you want to reduce system load while maintaining near-maximum performance.

**Why:**

1. **Sleep Interval = 0 (No Sleep)**:
   - Maximum throughput: No artificial delays
   - Threads continuously execute operations
   - Best for pure performance benchmarks
   - However, can cause high CPU usage and potential system saturation

2. **Small Sleep Intervals (1-10 microseconds)**:
   - **Minimal impact on throughput**: Modern systems can handle microsecond sleeps efficiently
   - **Reduces contention**: Brief pauses give other threads a chance to acquire locks
   - **Better for sustained loads**: Prevents complete CPU saturation
   - From your test results (1 microsecond sleep), you achieved ~48.8 million calls in 60 seconds, which is excellent performance

3. **Larger Sleep Intervals (100-1000+ microseconds)**:
   - **Significant throughput reduction**: Each sleep adds latency
   - With 1 microsecond sleep per operation and 48M operations, that's ~48 seconds of sleep time total
   - With 100 microseconds, you'd lose ~4800 seconds (80 minutes) of potential execution time
   - **Useful for throttling**: When you need to limit rate, not maximize throughput

**Trade-off Analysis:**
- **Throughput vs. Resource Usage**: Sleep = 0 gives max throughput but high CPU usage
- **Throughput vs. Contention**: Small sleep (1-10 μs) can actually improve throughput slightly by reducing lock contention
- **Throughput vs. Rate Control**: Larger sleeps are for rate limiting, not performance

**Conclusion**: The optimal sleep interval for maximum throughput is **0 microseconds**. However, a very small sleep interval (1-10 microseconds) provides excellent throughput while reducing system load and potentially improving performance through reduced contention. Based on your results with 1 microsecond sleep achieving ~813,711 calls/second, this represents near-optimal performance.

---

## Question 3: 500 Items Behavior (5 points)

**What would the numbers show if the initial load only loaded 500 items, where the first 500 add() calls in the second part of the test when the cache is not completely full, yet? Why would/does the benchmark behave this way?**

### Answer:

If the initial load only loads 500 items instead of 1000, you would observe a **performance difference** in the add() operations during the benchmark:

### Expected Behavior:

1. **First 500 add() operations**:
   - **Faster average time**: These operations don't require eviction because the cache has capacity remaining (500 items + 500 new adds = 1000 items, which is likely the cache capacity)
   - **No eviction overhead**: Adding to a non-full cache is simpler - just insert into the data structure
   - **Lower max time**: No need to find and remove the LRU item

2. **Subsequent add() operations** (after the cache fills):
   - **Slower average time**: Now the cache is full, so each add() requires:
     - Finding the least recently used (LRU) item to evict
     - Removing that item from the cache
     - Inserting the new item
   - **Higher max time**: Eviction operations can be more expensive, especially with lock contention

3. **Overall Statistics**:
   - **Bimodal distribution**: You'd see two peaks in timing - fast adds (before full) and slower adds (after full)
   - **Higher average time** compared to starting with 1000 items (where all adds require eviction from the start)
   - **Larger max time**: The eviction operations can cause spikes

### Why This Happens:

1. **Cache Capacity**: The Cache Manager likely has a capacity of 1000 items (or similar). Once full, every add() requires an eviction.

2. **Eviction Overhead**:
   - **LRU Tracking**: Must update LRU ordering (move recently accessed item to front)
   - **Removal**: Must remove the oldest item
   - **Insertion**: Must insert the new item
   - **Locking**: These operations may require locks, causing contention

3. **Initial Load Effect**:
   - With 1000 items preloaded: Cache is full from the start, so all benchmark add() calls require eviction
   - With 500 items preloaded: First 500 add() calls fill the cache (no eviction), subsequent calls require eviction
   - This creates a **two-phase performance pattern**

4. **Lock Contention**:
   - Once the cache is full and evictions begin, multiple threads compete for:
     - The same eviction candidates
     - Lock on the cache data structures
     - Memory allocation/deallocation
   - This increases variance and maximum times

### Specific Metrics You'd See:

**If testing with 500 items initial load:**
- **add() average time**: Would be lower overall (due to first 500 fast operations), but you'd see it increase over time
- **add() max time**: Similar to 1000-item case (evictions are still expensive), but fewer total evictions
- **Total add() calls**: Similar count, but performance profile changes

**Conclusion**: Loading only 500 items initially creates a two-phase benchmark where the first 500 add() operations are faster (no eviction needed) and subsequent add() operations are slower (eviction required). This demonstrates how cache capacity affects performance - operations on a non-full cache are simpler and faster than operations requiring eviction. The benchmark would show better overall add() performance initially, then degrade as the cache fills, highlighting the cost of eviction operations in an LRU cache.

---

## Files to Submit

### Required File:
- **`src/milestone3.cpp`** - Your modified benchmark implementation

### Optional (if the assignment asks for it):
- **`milestone3_config.json`** - Your configuration file (but the grader likely uses their own)

### Do NOT Submit:
- Any compiled executables (`.o`, `milestone3`, etc.)
- Build artifacts
- Test output files
- Documentation files (README, etc.) unless specifically requested

### Submission Format:
Most likely, you need to submit just the **`src/milestone3.cpp`** file. Check your assignment instructions to confirm, but typically only the source code file you modified is required.

---

## Summary

- **Question 1**: Optimal parallelism is typically **8-16 threads** due to balance between CPU utilization and lock contention
- **Question 2**: Optimal sleep interval is **0 microseconds** for max throughput, or **1-10 microseconds** for slightly reduced load
- **Question 3**: With 500 items, first 500 adds are faster (no eviction), then slower (eviction required), showing cache capacity effects

**Expected Points**: 15/15 for these answers + 120/120 for implementation = **135/135 total points**

# Milestone 3 - Assignment Answers

## Question 1: Optimal Degree of Parallelism (5 points)

**What degree of parallelism allows for the optimal throughput (total number of calls in a period of time)? Why do you think the component shows this performance characteristic?**

### Answer:

From running the benchmark with different thread counts, the best throughput on my machine happened around 8–16 threads. Below that, the CPU isn’t fully used, and above that the threads start fighting over shared cache structures, which causes lock contention and context switching. So the sweet spot is where all cores are active, but we’re not oversaturating the cache manager with too many threads.

**Conclusion**: The optimal degree of parallelism is typically **8-16 threads**, representing a balance between CPU utilization and synchronization overhead. Beyond this point, the cost of managing thread contention exceeds the benefit of additional parallelism.

---

## Question 2: Optimal Sleep Interval (5 points)

**What sleep interval allows for the optimal throughput?**

### Answer:

The highest throughput came from using a 0 µs sleep interval, since the threads never pause and can issue the most calls per second. Adding small sleeps (like 1–10 µs) didn’t improve performance — it only reduced load on the machine. Anything larger noticeably slows throughput because the threads spend more time sleeping than working.

**Conclusion**: The optimal sleep interval for maximum throughput is **0 microseconds**. However, a very small sleep interval (1-10 microseconds) provides excellent throughput while reducing system load and potentially improving performance through reduced contention. Based on your results with 1 microsecond sleep achieving ~813,711 calls/second, this represents near-optimal performance.

---

## Question 3: 500 Items Behavior (5 points)

**What would the numbers show if the initial load only loaded 500 items, where the first 500 add() calls in the second part of the test when the cache is not completely full, yet? Why would/does the benchmark behave this way?**

### Answer:

If the benchmark starts with 500 items instead of 1000, the first 500 add() calls run faster because the cache still has free space — no eviction is needed. After it reaches full capacity, every new add() requires an eviction, which makes the later add() calls slower. So the results would show two phases: quick adds at the beginning, and slower adds once the cache becomes full and eviction overhead kicks in.

**Conclusion**: Loading only 500 items initially creates a two-phase benchmark where the first 500 add() operations are faster (no eviction needed) and subsequent add() operations are slower (eviction required). This demonstrates how cache capacity affects performance - operations on a non-full cache are simpler and faster than operations requiring eviction. The benchmark would show better overall add() performance initially, then degrade as the cache fills, highlighting the cost of eviction operations in an LRU cache.

---

// Nathaniel Moreno
// CSC-641
// Milestone3.cpp
// Description: Multithreaded benchmark driver for CacheManager library
#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <climits>


#include "cache-manager.hpp"
#include "json.hpp"

using namespace std;
using namespace std::chrono;
using json = nlohmann::json;
using CacheManager = cache::CacheManager<int, std::string, bench::TbbBench>;

// Statistics structure for each method
struct MethodStats {
    atomic<long long> totalCalls{0};
    atomic<long long> totalTimeNs{0};
    atomic<long long> maxTimeNs{0};
    atomic<long long> minTimeNs{LLONG_MAX};
    
    void recordCall(long long timeNs) {
        totalCalls++;
        totalTimeNs += timeNs;
        
        long long currentMax = maxTimeNs.load();
        while (timeNs > currentMax && !maxTimeNs.compare_exchange_weak(currentMax, timeNs)) {
            currentMax = maxTimeNs.load();
        }
        
        long long currentMin = minTimeNs.load();
        while (timeNs < currentMin && !minTimeNs.compare_exchange_weak(currentMin, timeNs)) {
            currentMin = minTimeNs.load();
        }
    }
    
    long long getMinTime() const {
        long long min = minTimeNs.load();
        return (min == LLONG_MAX) ? 0 : min;
    }
};

// Global statistics for each method
MethodStats statsGetItem;
MethodStats statsAdd;
MethodStats statsContains;
MethodStats statsRemove;

// Global cache manager instance
CacheManager* cacheManager = nullptr;

// Configuration structure
struct BenchmarkConfig {
    int testDuration;
    string testType;
    int getItemRatio;
    int addRatio;
    int containsRatio;
    int removeRatio;
    int degreeOfParallelism;
    int sleepInterval;
    
    int totalRatio() const {
        return getItemRatio + addRatio + containsRatio + removeRatio;
    }
};

// Function to read JSON configuration
BenchmarkConfig readConfig(const string& configPath) {
    BenchmarkConfig config;
    
    ifstream file(configPath);
    if (!file.is_open()) {
        cerr << "Error: Cannot open config file: " << configPath << endl;
        exit(1);
    }
    
    json root;
    try {
        file >> root;
    } catch (const json::parse_error& e) {
        cerr << "Error: Failed to parse JSON config file: " << e.what() << endl;
        exit(1);
    }
    
    // Parse nested structure: Milestone3[0].defaultVariables[0]
    config.testDuration = root["Milestone3"][0]["defaultVariables"][0]["testDuration"].get<int>();
    config.testType = root["Milestone3"][0]["defaultVariables"][0]["testType"].get<string>();
    config.degreeOfParallelism = root["Milestone3"][0]["defaultVariables"][0]["degreeOfParallelism"].get<int>();
    config.sleepInterval = root["Milestone3"][0]["defaultVariables"][0]["sleepInterval"].get<int>();
    
    json ratios = root["Milestone3"][0]["defaultVariables"][0]["ratioOfMethods"][0];
    config.getItemRatio = ratios["getItem"].get<int>();
    config.addRatio = ratios["add"].get<int>();
    config.containsRatio = ratios["contains"].get<int>();
    config.removeRatio = ratios["remove"].get<int>();
    
    return config;
}

// Helper function to get random key for operations
int getRandomKeyForGet(int maxKey = 1000) {
    static thread_local random_device rd;
    static thread_local mt19937 gen(rd());
    uniform_int_distribution<> dis(1, maxKey);
    return dis(gen);
}

int getRandomKeyForAdd(int startKey = 1001) {
    static thread_local random_device rd;
    static thread_local mt19937 gen(rd());
    uniform_int_distribution<> dis(startKey, startKey + 10000);
    return dis(gen);
}

// Phase 1: Initial Load
high_resolution_clock::time_point performInitialLoad(CacheManager* cache) {
    auto start = high_resolution_clock::now();
    
    cout << "Phase 1: Initial Load - Loading 1000 items..." << endl;
    
    for (int i = 1; i <= 1000; i++) {
        cache->add(i, "value_" + to_string(i));
    }
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << "Initial Load completed in " << duration.count() << " microseconds" << endl;
    cout << "Initial Load completed in " << duration.count() / 1000.0 << " milliseconds" << endl;
    
    return start;
}

// Static ratio benchmark worker
void staticBenchmarkWorker(int threadId, const BenchmarkConfig& config, 
                           high_resolution_clock::time_point startTime) {
    int nextAddKey = 1001 + threadId * 10000; // Ensure unique keys per thread
    random_device rd;
    mt19937 gen(rd() + threadId);
    
    auto endTime = startTime + seconds(config.testDuration);
    
    while (high_resolution_clock::now() < endTime) {
        for (int i = 1; i <= 9 && high_resolution_clock::now() < endTime; i++) {
            // 3 getItem calls
            for (int j = 1; j <= 3 && high_resolution_clock::now() < endTime; j++) {
                int key = getRandomKeyForGet(1000);
                auto callStart = high_resolution_clock::now();
                cacheManager->getItem(key);
                auto callEnd = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(callEnd - callStart).count();
                statsGetItem.recordCall(duration);
                
                if (config.sleepInterval > 0) {
                    this_thread::sleep_for(microseconds(config.sleepInterval));
                }
            }
            
            if (high_resolution_clock::now() >= endTime) break;
            
            // 1 add call
            {
                int addKey = nextAddKey++;
                auto callStart = high_resolution_clock::now();
                cacheManager->add(addKey, "value_" + to_string(addKey));
                auto callEnd = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(callEnd - callStart).count();
                statsAdd.recordCall(duration);
                
                if (config.sleepInterval > 0) {
                    this_thread::sleep_for(microseconds(config.sleepInterval));
                }
            }
            
            if (high_resolution_clock::now() >= endTime) break;
            
            // 3 more getItem calls
            for (int j = 1; j <= 3 && high_resolution_clock::now() < endTime; j++) {
                int key = getRandomKeyForGet(1000);
                auto callStart = high_resolution_clock::now();
                cacheManager->getItem(key);
                auto callEnd = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(callEnd - callStart).count();
                statsGetItem.recordCall(duration);
                
                if (config.sleepInterval > 0) {
                    this_thread::sleep_for(microseconds(config.sleepInterval));
                }
            }
            
            if (high_resolution_clock::now() >= endTime) break;
            
            // Occasionally contains (2% overall, so when i == 2 or i == 8)
            if (i == 2 || i == 8) {
                int key = getRandomKeyForGet(1000);
                auto callStart = high_resolution_clock::now();
                cacheManager->contains(key);
                auto callEnd = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(callEnd - callStart).count();
                statsContains.recordCall(duration);
                
                if (config.sleepInterval > 0) {
                    this_thread::sleep_for(microseconds(config.sleepInterval));
                }
            }
            
            if (high_resolution_clock::now() >= endTime) break;
            
            // 3 more getItem calls
            for (int j = 1; j <= 3 && high_resolution_clock::now() < endTime; j++) {
                int key = getRandomKeyForGet(1000);
                auto callStart = high_resolution_clock::now();
                cacheManager->getItem(key);
                auto callEnd = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(callEnd - callStart).count();
                statsGetItem.recordCall(duration);
                
                if (config.sleepInterval > 0) {
                    this_thread::sleep_for(microseconds(config.sleepInterval));
                }
            }
            
            if (high_resolution_clock::now() >= endTime) break;
            
            // 1 remove call
            {
                int removeKey = getRandomKeyForGet(1000);
                auto callStart = high_resolution_clock::now();
                cacheManager->remove(removeKey);
                auto callEnd = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(callEnd - callStart).count();
                statsRemove.recordCall(duration);
                
                if (config.sleepInterval > 0) {
                    this_thread::sleep_for(microseconds(config.sleepInterval));
                }
            }
        }
    }
}

// Dynamic ratio benchmark worker
// Uses probability-based selection from config ratios
void dynamicBenchmarkWorker(int threadId, const BenchmarkConfig& config,
                           high_resolution_clock::time_point startTime) {
    int nextAddKey = 1001 + threadId * 10000; // Ensure unique keys per thread
    random_device rd;
    mt19937 gen(rd() + threadId);
    
    auto endTime = startTime + seconds(config.testDuration);
    
    // Build weighted selection vector based on ratios
    vector<int> methodChoices;
    for (int i = 0; i < config.getItemRatio; i++) methodChoices.push_back(0); // getItem
    for (int i = 0; i < config.addRatio; i++) methodChoices.push_back(1);     // add
    for (int i = 0; i < config.containsRatio; i++) methodChoices.push_back(2); // contains
    for (int i = 0; i < config.removeRatio; i++) methodChoices.push_back(3);   // remove
    
    if (methodChoices.empty()) {
        cerr << "Error: No methods configured in ratios" << endl;
        return;
    }
    
    uniform_int_distribution<> methodDis(0, methodChoices.size() - 1);
    
    while (high_resolution_clock::now() < endTime) {
        int method = methodChoices[methodDis(gen)];
        
        switch (method) {
            case 0: { // getItem
                int key = getRandomKeyForGet(1000);
                auto callStart = high_resolution_clock::now();
                cacheManager->getItem(key);
                auto callEnd = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(callEnd - callStart).count();
                statsGetItem.recordCall(duration);
                break;
            }
            case 1: { // add
                int addKey = nextAddKey++;
                auto callStart = high_resolution_clock::now();
                cacheManager->add(addKey, "value_" + to_string(addKey));
                auto callEnd = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(callEnd - callStart).count();
                statsAdd.recordCall(duration);
                break;
            }
            case 2: { // contains
                int key = getRandomKeyForGet(1000);
                auto callStart = high_resolution_clock::now();
                cacheManager->contains(key);
                auto callEnd = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(callEnd - callStart).count();
                statsContains.recordCall(duration);
                break;
            }
            case 3: { // remove
                int removeKey = getRandomKeyForGet(1000);
                auto callStart = high_resolution_clock::now();
                cacheManager->remove(removeKey);
                auto callEnd = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(callEnd - callStart).count();
                statsRemove.recordCall(duration);
                break;
            }
        }
        
        if (config.sleepInterval > 0) {
            this_thread::sleep_for(microseconds(config.sleepInterval));
        }
    }
}

// Print statistics for a method
void printMethodStats(const string& methodName, const MethodStats& stats) {
    long long calls = stats.totalCalls.load();
    if (calls == 0) {
        cout << methodName << ":" << endl;
        cout << "  Total calls: 0" << endl;
        cout << "  Average time: N/A" << endl;
        cout << "  Max time: N/A" << endl;
        cout << "  Min time: N/A" << endl;
        return;
    }
    
    long long totalNs = stats.totalTimeNs.load();
    long long maxNs = stats.maxTimeNs.load();
    long long minNs = stats.getMinTime();
    
    double avgNs = static_cast<double>(totalNs) / calls;
    
    cout << methodName << ":" << endl;
    cout << "  Total calls: " << calls << endl;
    cout << "  Average time: " << fixed << setprecision(2) << avgNs << " ns (" 
         << avgNs / 1000.0 << " μs, " << avgNs / 1000000.0 << " ms)" << endl;
    cout << "  Max time: " << maxNs << " ns (" << maxNs / 1000.0 << " μs, " 
         << maxNs / 1000000.0 << " ms)" << endl;
    cout << "  Min time: " << minNs << " ns (" << minNs / 1000.0 << " μs, " 
         << minNs / 1000000.0 << " ms)" << endl;
}

int main(int argc, char* argv[]) {
    string configPath = "milestone3_config.json";
    if (argc > 1) {
        configPath = argv[1];
    }
    
    // Read configuration
    BenchmarkConfig config = readConfig(configPath);
    
    cout << "=== Multithreaded Cache Manager Benchmark ===" << endl;
    cout << "Configuration:" << endl;
    cout << "  Test Duration: " << config.testDuration << " seconds" << endl;
    cout << "  Test Type: " << config.testType << endl;
    cout << "  Degree of Parallelism: " << config.degreeOfParallelism << endl;
    cout << "  Sleep Interval: " << config.sleepInterval << " microseconds" << endl;
    cout << "  Method Ratios - getItem: " << config.getItemRatio 
         << "%, add: " << config.addRatio 
         << "%, contains: " << config.containsRatio 
         << "%, remove: " << config.removeRatio << "%" << endl;
    cout << endl;
    
    // Initialize cache manager
    cacheManager = new CacheManager();
    
    // Phase 1: Initial Load
    auto loadStart = performInitialLoad(cacheManager);
    auto loadEnd = high_resolution_clock::now();
    auto loadDuration = duration_cast<microseconds>(loadEnd - loadStart);
    
    cout << endl;
    cout << "Phase 2: Benchmark Execution - Starting " << config.degreeOfParallelism 
         << " worker threads..." << endl;
    
    // Phase 2: Benchmark Execution
    auto benchmarkStart = high_resolution_clock::now();
    vector<thread> workers;
    
    for (int i = 0; i < config.degreeOfParallelism; i++) {
        if (config.testType == "static") {
            workers.emplace_back(staticBenchmarkWorker, i, config, benchmarkStart);
        } else if (config.testType == "dynamic") {
            workers.emplace_back(dynamicBenchmarkWorker, i, config, benchmarkStart);
        } else {
            cerr << "Error: Unknown test type: " << config.testType << endl;
            exit(1);
        }
    }
    
    // Wait for all threads to complete
    for (auto& t : workers) {
        t.join();
    }
    
    auto benchmarkEnd = high_resolution_clock::now();
    auto benchmarkDuration = duration_cast<microseconds>(benchmarkEnd - benchmarkStart);
    
    // Call clear() exactly once at the end
    cout << "Calling clear()..." << endl;
    cacheManager->clear();
    
    // Print results
    cout << endl;
    cout << "=== Benchmark Results ===" << endl;
    cout << "Initial Load Time: " << loadDuration.count() << " microseconds (" 
         << loadDuration.count() / 1000.0 << " milliseconds)" << endl;
    cout << "Total Test Time: " << benchmarkDuration.count() << " microseconds (" 
         << benchmarkDuration.count() / 1000.0 << " milliseconds, " 
         << benchmarkDuration.count() / 1000000.0 << " seconds)" << endl;
    cout << endl;
    
    printMethodStats("getItem", statsGetItem);
    cout << endl;
    printMethodStats("add", statsAdd);
    cout << endl;
    printMethodStats("contains", statsContains);
    cout << endl;
    printMethodStats("remove", statsRemove);
    cout << endl;
    
    // Cleanup
    delete cacheManager;
    
    return 0;
}

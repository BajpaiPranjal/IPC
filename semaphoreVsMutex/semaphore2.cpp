// g++ -std=c++20 semaphore2.cpp -pthread -o semaphore
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <semaphore>
#include <chrono>

using namespace std;
using namespace std::chrono_literals;

const int BUFFER_SIZE = 5;     // Max buffer capacity
const int NUM_PRODUCERS = 2;   // Number of producer threads
const int NUM_CONSUMERS = 3;   // Number of consumer threads
const int ITEMS_TO_PRODUCE = 10;

// Shared buffer
queue<int> buffer;

// Semaphores
counting_semaphore<BUFFER_SIZE> emptySlots(BUFFER_SIZE); // Tracks available slots
counting_semaphore<BUFFER_SIZE> fullSlots(0);            // Tracks filled slots

// Mutex to protect buffer access
mutex bufferMutex;

// Producer function
void producer(int id) {
    for (int i = 0; i < ITEMS_TO_PRODUCE; ++i) {
        int item = id * 100 + i; // Unique item per producer

        emptySlots.acquire();  // Wait for empty slot
        {
            lock_guard<mutex> lock(bufferMutex);
            buffer.push(item);
            cout << "Producer " << id << " produced item " << item << endl;
        }
        fullSlots.release();   // Signal item added
        this_thread::sleep_for(100ms); // Simulate work
    }
}

// Consumer function
void consumer(int id) {
    while (true) {
        fullSlots.acquire();   // Wait for item
        int item = -1;
        {
            lock_guard<mutex> lock(bufferMutex);
            if (!buffer.empty()) {
                item = buffer.front();
                buffer.pop();
                cout << "Consumer " << id << " consumed item " << item << endl;
            }
        }
        emptySlots.release(); // Signal slot freed

        if (item == -1) break; // End condition (optional)
        this_thread::sleep_for(150ms); // Simulate work
    }
}

int main() {
    vector<thread> producers, consumers;

    // Launch producers
    for (int i = 0; i < NUM_PRODUCERS; ++i)
        producers.emplace_back(producer, i);

    // Launch consumers
    for (int i = 0; i < NUM_CONSUMERS; ++i)
        consumers.emplace_back(consumer, i);

    // Join producers
    for (auto& p : producers)
        p.join();

    // After all producers are done, insert dummy items to stop consumers
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        emptySlots.acquire();
        lock_guard<mutex> lock(bufferMutex);
        buffer.push(-1); // Special item meaning "stop"
        fullSlots.release();
    }

    // Join consumers
    for (auto& c : consumers)
        c.join();

    return 0;
}

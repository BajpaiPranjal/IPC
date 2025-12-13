#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <unordered_map>

using namespace std;

#define ct cout
#define el endl

#include <stack>
#include <set>
#include <algorithm>

#define f(var, a, b) for (int var = a; var < b; var++)

#include <mutex>
#include <thread>
#include <semaphore>

// Compile using : g++ -std=c++20 simpleSemaphore.cpp -pthread -o app

#pragma region MUTEX
/*
mutex mtx; // only one thread can lock at a time

 void printTask(int id) {
    mtx.lock(); // acquire lock
    cout << "Thread " << id << " is working...\n";
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Thread " << id << " finished.\n";
    mtx.unlock(); // release lock
}

int main() {
    thread t1(printTask, 1);
    thread t2(printTask, 2);
    thread t3(printTask, 3);

    t1.join();
    t2.join();
    t3.join();
} */

#pragma endregion

#pragma region SEMAPHORE

/* counting_semaphore<2> sem(4);
// allow 2 threads at the same time

void printTask(int id)
{
    sem.acquire(); // take a "token"
    cout << "Thread " << id << " is working...\n";
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Thread " << id << " finished.\n";
    sem.release(); // return token
}

int main()
{
    thread t1(printTask, 1);
    thread t2(printTask, 2);
    thread t3(printTask, 3);
    thread t4(printTask, 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
} */

#pragma endregion

#pragma region DB Example

/*
Suppose you have a database that only allows 3 connections at a time.
You’ve got 10 threads (clients), but only 3 should connect concurrently.

That’s exactly where a counting semaphore fits:
*/


counting_semaphore<3> dbConnections(3);

void accessDatabase(int id) {
    dbConnections.acquire(); // Wait if pool is full
    cout << "Client " << id << " connected to DB\n";
    this_thread::sleep_for(chrono::seconds(2)); // simulate work
    cout << "Client " << id << " disconnected\n";
    dbConnections.release(); // Free slot
}

int main() {
    thread clients[10];
    for (int i = 0; i < 10; i++) {
        clients[i] = thread(accessDatabase, i + 1);
    }
    for (int i = 0; i < 10; i++) {
        clients[i].join();
    }
}
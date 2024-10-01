#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;

const int MAX_ONES_TARGET = 177; // Target for max ones
mutex mtx; // Mutex for thread safety
int maxOnes = 0; // Shared variable to track the maximum ones

void rollDice(long long rollsToPerform, int threadID) 
{
    vector<int> numbers(4, 0); // Local count for this thread

    random_device rd; // Obtain a random number from hardware
    mt19937 generator(rd()); // Seed the generator
    uniform_int_distribution<> distribution(1, 4);

    long long rolls = 0;

    while (maxOnes < MAX_ONES_TARGET && rolls < rollsToPerform) 
    {
        rolls++;
        for (int i = 0; i < 231; ++i) 
        {
            int roll = distribution(generator); // Generate random number
            numbers[roll - 1] += 1; // Increment the count
        }

        mtx.lock(); // Lock the mutex to update shared variable
        if (numbers[0] > maxOnes)
        {
            maxOnes = numbers[0];
            cout << "Thread " << threadID << ": New maxOnes = " << maxOnes << endl;
            numbers = { 0, 0, 0, 0 };
        }
        else
        {
            numbers = { 0, 0, 0, 0 };//more efficient to have this value reset while in the if else statement
        }
        mtx.unlock(); // Unlock the mutex  
    }
}

int main() 
{
    long long totalRolls;
    long long numThreads;


    cout << "Enter the total number of rolls to perform: ";
    cin >> totalRolls;
    cout << "!!!Caution if you are running this on a potato keep the number under 500!!!" << endl;
    cout << "Enter the number of threads you would like to use: ";
    cin >> numThreads;
    cout << "Hold onto your butts..." << endl;

    // Starting timer
    auto start_time = chrono::high_resolution_clock::now();

    // Create threads
    vector<thread> threads;
    long long rollsPerThread = totalRolls / numThreads;

    for (int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(rollDice, rollsPerThread, i);
    }

    // Join threads
    for (auto& t : threads) 
    {
        t.join();
    }

    // Ending timer
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end_time - start_time;

    cout << "Highest Ones Roll: " << maxOnes << endl;
    cout << "Execution Time: " << duration.count() << " seconds" << endl;
    system("pause");
    cout << "Are you sure you have your values?" << endl;
    system("pause");

    return 0;
}

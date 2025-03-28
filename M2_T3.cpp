#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>

using namespace std;

const int NUM_PRODUCERS = 2;
const int NUM_CONSUMERS = 2;
const int NUM_LIGHTS = 5;
const int MEASUREMENTS_PER_HOUR = 12;
const int TOP_N = 3;
const int TOP_DISPLAY = 4; // Extract top 4 from file

struct TrafficData {
    int timestamp;
    int light_id;
    int cars_passed;
};

queue<TrafficData> trafficQueue;
vector<vector<TrafficData>> trafficData;
mutex mtx;
condition_variable cv;
bool done = false;

void producer(int producer_id) {
    srand(time(0) + producer_id);
    for (int timestamp = 0; timestamp < 60; timestamp += 5) {
        for (int light_id = 0; light_id < NUM_LIGHTS; light_id++) {
            TrafficData data = {timestamp, light_id, rand() % 100 + 1};
            {
                unique_lock<mutex> lock(mtx);
                trafficQueue.push(data);
            }
            cv.notify_one();
            this_thread::sleep_for(chrono::milliseconds(100 + rand() % 200));
        }
    }
}

void consumer(int consumer_id) {
    vector<TrafficData> localData;
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return !trafficQueue.empty() || done; });

        if (trafficQueue.empty() && done) break;

        TrafficData data = trafficQueue.front();
        trafficQueue.pop();
        
        localData.push_back(data);
    }

    sort(localData.begin(), localData.end(), [](TrafficData a, TrafficData b) {
        return a.cars_passed > b.cars_passed;
    });

    localData.resize(min(TOP_N, (int)localData.size()));

    {
        unique_lock<mutex> lock(mtx);
        trafficData.push_back(localData);
    }
}

void writeToFile() {
    ofstream file("traffic_data.txt");
    for (const auto& hourlyData : trafficData) {
        for (const auto& data : hourlyData) {
            file << data.timestamp << " " << data.light_id << " " << data.cars_passed << "\n";
        }
    }
    file.close();
}


void readFromFile() {
    ifstream file("traffic_data.txt");
    if (!file) {
        cerr << "Error: Unable to open traffic_data.txt file!\n";
        return;
    }
    
    vector<TrafficData> extractedData;
    int timestamp, light_id, cars_passed;

    while (file >> timestamp >> light_id >> cars_passed) {
        extractedData.push_back({timestamp, light_id, cars_passed});
    }
    
    file.close();

    // Sort the extracted data to find the top 4 most congested lights
    sort(extractedData.begin(), extractedData.end(), [](TrafficData a, TrafficData b) {
        return a.cars_passed > b.cars_passed;
    });

    cout << "\n=== Top 4 Most Congested Traffic Lights ===\n";
    for (int i = 0; i < min(TOP_DISPLAY, (int)extractedData.size()); i++) {
        cout << "Timestamp: " << extractedData[i].timestamp << " min | ";
        cout << "Light ID: " << extractedData[i].light_id << " | ";
        cout << "Cars Passed: " << extractedData[i].cars_passed << "\n";
    }
}

int main() {
    vector<thread> producerThreads, consumerThreads;
    
    for (int i = 0; i < NUM_PRODUCERS; i++)
        producerThreads.emplace_back(producer, i);
    
    for (int i = 0; i < NUM_CONSUMERS; i++)
        consumerThreads.emplace_back(consumer, i);
    
    for (auto& t : producerThreads)
        t.join();
    
    {
        unique_lock<mutex> lock(mtx);
        done = true;
    }
    cv.notify_all();
    
    for (auto& t : consumerThreads)
        t.join();
    
    writeToFile();
    cout << "Traffic data has been processed and saved to 'traffic_data.txt'.\n";

    
    readFromFile();

    return 0;
}

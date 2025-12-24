#pragma once
#include <mutex>
#include <condition_variable>
#include <map>

class SyncManager {
private:
    std::mutex mtx;
    std::condition_variable cv;
    std::map<int, int> readersCount; 
    std::map<int, bool> isWriting;   

public:
    void lockReader(int id);
    void unlockReader(int id);
    void lockWriter(int id);
    void unlockWriter(int id);
};
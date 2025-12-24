#include "SyncManager.h"
#include <iostream>

void SyncManager::lockReader(int id) {
    std::unique_lock<std::mutex> lock(mtx);
   
    cv.wait(lock, [this, id]() {
        return !isWriting[id];
        });
    readersCount[id]++;
}

void SyncManager::unlockReader(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    readersCount[id]--;
    if (readersCount[id] == 0) {
        cv.notify_all(); 
    }
}

void SyncManager::lockWriter(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this, id]() {
        return !isWriting[id] && readersCount[id] == 0;
        });
    isWriting[id] = true;
}

void SyncManager::unlockWriter(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    isWriting[id] = false;
    cv.notify_all(); 
}
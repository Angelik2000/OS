#include <gtest/gtest.h>
#include "common/IPCQueue.hpp"
#include <filesystem>
#include <windows.h> 

class IPCQueueTest : public ::testing::Test {
protected:
    std::wstring testFile = L"test_q.bin";

    void SetUp() override {
        if (std::filesystem::exists(testFile)) {
            std::filesystem::remove(testFile);
        }
    }

    void TearDown() override {
        if (std::filesystem::exists(testFile)) {
            std::filesystem::remove(testFile);
        }
    }
};

TEST_F(IPCQueueTest, SimplePushPop) {
    lab::ipc::IPCQueue q(testFile, 10);
    q.Push("Test");
    EXPECT_EQ("Test", q.Pop());
}

TEST_F(IPCQueueTest, FIFO_Order) {
    lab::ipc::IPCQueue q(testFile, 10);
    q.Push("1");
    q.Push("2");
    EXPECT_EQ("1", q.Pop());
    EXPECT_EQ("2", q.Pop());
}

TEST_F(IPCQueueTest, CircularWrapAround) {
    lab::ipc::IPCQueue q(testFile, 2);
    q.Push("A");
    q.Push("B");
    (void)q.Pop(); 
    q.Push("C");   
    EXPECT_EQ("B", q.Pop());
    EXPECT_EQ("C", q.Pop());
}
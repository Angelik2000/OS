#include <gtest/gtest.h>
#include "../src/SharedData.h"
#include "../src/Utils.h"

class LabTestFixture : public ::testing::Test {};

TEST_F(LabTestFixture, SharedDataInitialization) {
    SharedData data(10);
    EXPECT_EQ(data.array.size(), 10);
    for (int val : data.array) EXPECT_EQ(val, 0);
}

TEST_F(LabTestFixture, ScopedHandleThrowsOnInvalid) {
    EXPECT_THROW(Utils::MakeScopedHandle(INVALID_HANDLE_VALUE), std::runtime_error);
}

TEST_F(LabTestFixture, CriticalSectionGuard) {
    SharedData data(5);
    {
        Utils::CriticalSectionGuard lock(data.csArrayAccess);
        data.array[0] = 100;
    }
    EXPECT_EQ(data.array[0], 100);
}

TEST_F(LabTestFixture, ArrayClearingLogic) {
    SharedData data(3);
    data.array = { 1, 2, 1 };
    int idToRemove = 1;

    for (int& val : data.array) {
        if (val == idToRemove) val = 0;
    }

    std::vector<int> expected = { 0, 2, 0 };
    EXPECT_EQ(data.array, expected);
}
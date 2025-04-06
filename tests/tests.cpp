#include "thread_safe_vector.hpp"
#include <gtest/gtest.h>
#include <thread>

class ThreadSafeVectorTest : public ::testing::Test {
protected:
    ThreadSafeVector<int> vec;

    void SetUp() override {
        vec.push_back(10);
        vec.push_back(20);
        vec.push_back(30);
    }
};

TEST_F(ThreadSafeVectorTest, PushBackIncreasesSize) {
    size_t initial_size = vec.size();
    vec.push_back(40);
    EXPECT_EQ(vec.size(), initial_size + 1);
}

TEST_F(ThreadSafeVectorTest, PopBackDecreasesSize) {
    size_t initial_size = vec.size();
    vec.pop_back();
    EXPECT_EQ(vec.size(), initial_size - 1);
}
TEST(ThreadSafeVector, PopBackEmptyDoesNothing) {
    ThreadSafeVector<int> empty_vec;
    empty_vec.pop_back();
    EXPECT_EQ(empty_vec.size(), 0);
}

TEST_F(ThreadSafeVectorTest, EraseRemovesElement) {
    vec.erase(1);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[1], 30);
}

TEST(ThreadSafeVector, EraseOutOfBoundsThrows) {
    ThreadSafeVector<int> vec;
    vec.push_back(10);
    EXPECT_THROW(vec.erase(1), std::out_of_range);
}

TEST_F(ThreadSafeVectorTest, BracketOperatorAccess) {
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
}

TEST_F(ThreadSafeVectorTest, AtFunctionAccess) {
    EXPECT_EQ(vec.at(2), 30);
    EXPECT_THROW(vec.at(10), std::out_of_range);
}

TEST_F(ThreadSafeVectorTest, ClearEmptiesVector) {
    vec.clear();
    EXPECT_EQ(vec.size(), 0);
    EXPECT_TRUE(vec.empty());
}

TEST_F(ThreadSafeVectorTest, EmptyFunction) {
    ThreadSafeVector<int> empty_vec;
    EXPECT_TRUE(empty_vec.empty());

    empty_vec.push_back(5);
    EXPECT_FALSE(empty_vec.empty());
}

TEST(ThreadSafeVector, ResizeIncreasesCapacity) {
    ThreadSafeVector<int> vec(2);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    EXPECT_EQ(vec.size(), 3);
}

TEST(ThreadSafeVector, ConcurrentPushBack) {
    ThreadSafeVector<int> vec;

    auto task = [&vec]() {
        for (int i = 0; i < 100; ++i) {
            vec.push_back(i);
        }
    };

    std::thread t1(task);
    std::thread t2(task);

    t1.join();
    t2.join();

    EXPECT_EQ(vec.size(), 200);
}

TEST(ThreadSafeVector, ConcurrentReadAccess) {
    ThreadSafeVector<int> vec;
    for (int i = 0; i < 100; ++i) {
        vec.push_back(i);
    }

    auto read_task = [&vec]() {
        for (int i = 0; i < 100; ++i) {
            vec.at(i);
        }
    };

    std::thread t1(read_task);
    std::thread t2(read_task);

    t1.join();
    t2.join();

    EXPECT_EQ(vec.size(), 100);
}

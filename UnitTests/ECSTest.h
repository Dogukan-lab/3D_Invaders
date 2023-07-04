#pragma once
#include "gtest/gtest.h"
#include "Coordinator.h"

struct MockComp1 {
    std::pair<int,int> position;
};

class ECSTest : public ::testing::Test {
public:
    ECSTest() {
        ecsCoordinator = new Coordinator();
    };

    ~ECSTest() {
        delete ecsCoordinator;
        ecsCoordinator = nullptr;
    }

    void SetUp() {
        ecsCoordinator = new Coordinator();
    }
    void TearDown() {
        delete ecsCoordinator;
        ecsCoordinator = nullptr;
    }

    Coordinator* ecsCoordinator;
};
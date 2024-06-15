#include "UnitTester.h"
#include "TestLogger.h"
#include <fstream>

int UnitTester::startTests() {
    ::testing::InitGoogleTest();

    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    listeners.Append(new TestLogger);

    int result = RUN_ALL_TESTS();

    return result;
}

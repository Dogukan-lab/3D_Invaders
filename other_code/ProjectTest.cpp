#include "gtest/gtest.h"
#include "MovingObject.h"

#include <fstream>

class ProjectTest : public testing::Test
{
	protected:
		MovingObject* testObject {};

		virtual void SetUp()
		{
			testObject = new MovingObject();
		}

		virtual void TearDown()
		{
			delete testObject;
		}
};

TEST_F(ProjectTest, TestAddition)
{
	std::ifstream reader;
	reader.open("testLog.txt", std::ios::in);
	if (reader.is_open())
	{
		std::string line;
		std::getline(reader, line);

		reader.close();
		const int expected = atoi(line.c_str());

		testObject->Translate(expected, 0, 0);
		ASSERT_EQ(testObject->GetPosition().x, expected);
	} else
	{
		throw std::runtime_error("Could not open file");
	}
}

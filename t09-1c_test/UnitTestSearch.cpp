#include "stdafx.h"
#include "CppUnitTest.h"
#include "SearchParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace t091c_test
{
	TEST_CLASS(UnitTest2)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			SearchParser test;
			string argument = test.findTypeAndGetArgument("date 111014");//equivalence partitioning for valid values
			string expectedArgument = "111014";
			Assert::AreEqual(expectedArgument,argument);
		}
		TEST_METHOD(TestMethod2a)
		{
			SearchParser test;
			string argument = test.findTypeAndGetArgument("date 11101");//equivalence partitioning for 
			test.extractDate(argument);
			string errorString = SEARCH_PARSER_6DIGIT_DATE_ERROR;
			Assert::AreEqual(errorString, test.getErrorString());
		}
		TEST_METHOD(TestMethod2b)
		{
			SearchParser test;
			string argument = test.findTypeAndGetArgument("date 1110141");//for longer strings
			test.extractDate(argument);
			string errorString = SEARCH_PARSER_6DIGIT_DATE_ERROR;
			Assert::AreEqual(errorString, test.getErrorString());
		}
		TEST_METHOD(TestMethod2c)
		{
			SearchParser test;
			string argument = test.findTypeAndGetArgument("date 111014  ");//for white space
			string date = test.extractDate(argument);
			string testDate = "11/10/2014";
			Assert::AreEqual(testDate, date);
		}
		TEST_METHOD(TestMethod3a)
		{
			SearchParser test;
			string argument = test.findTypeAndGetArgument("date Monday");//have to repeat for all valid
			string date = test.extractDate(argument);
			string testDate = "27/10/2014";
			Assert::AreEqual(testDate, date);
		}
		TEST_METHOD(TestMethod3b)
		{
			SearchParser test;
			string argument = test.findTypeAndGetArgument("date Grunday");//for all wrong strings
			test.extractDate(argument);
			string errorString = SEARCH_PARSER_DAY_OF_WEEK_ERROR;
			Assert::AreEqual(errorString, test.getErrorString());
		}
		/*ParsedDataPackage SearchParser::parseAndReturn(string parseInput)
		{
			string argument = findTypeAndGetArgument(parseInput);
			if (type == "date"){
				parsedData.date = extractDate(argument);
			}
			else if (type == "name"){
				parsedData.name = argument;
			}
			else if (type == "category"){
				parsedData.category = argument;
			}
			return parsedData;
		}*/
	};
}
#include "CppUnitTest.h"
#include "Text.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WntTest
{
	TEST_CLASS(WntStringTest)
	{
	public:

		TEST_METHOD(TestAsWString)
		{
			WNT_STRING s("AAAAA");
			s.AsWString();
		}
	};
}
